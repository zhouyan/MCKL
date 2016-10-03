#!/usr/bin/perl

# ============================================================================
#  MCKL/manual/tab/random_distribution.pl
# ----------------------------------------------------------------------------
#  MCKL: Monte Carlo Kernel Library
# ----------------------------------------------------------------------------
#  Copyright (c) 2013-2016, Yan Zhou
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#    Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
#    Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
# ============================================================================

use v5.16;
use Getopt::Long;

my $run = 0;
my $pdf = 0;
my $build = 0;
my $simd;
my $llvm = "../../build/llvm-release-sys";
my $gnu = "../../build/gnu-release-sys";
my $intel = "../../build/intel-release-sys";
my $make = "ninja";
my $name;
my $write = 0;
GetOptions(
    "run"      => \$run,
    "pdf"      => \$pdf,
    "build"    => \$build,
    "simd=s"   => \$simd,
    "llvm=s"   => \$llvm,
    "gnu=s"    => \$gnu,
    "intel=s"  => \$intel,
    "make=s"   => \$make,
    "name=s"   => \$name,
    "write"    => \$write,
);

if ($simd) {
    $run = 0;
    $build = 0;
} else {
    my $cpuid = `cpuid_info`;
    $simd = "sse2" if $cpuid =~ "SSE2";
    $simd = "avx2" if $cpuid =~ "AVX2";
}

my $all = 0;
$all = 1 if not $name or $name =~ /all/;

my %build_dir = (llvm => $llvm, gnu => $gnu, intel => $intel);

my @inverse = qw(Arcsine Cauchy Exponential ExtremeValue Laplace Logistic
Pareto Rayleigh UniformReal Weibull);
my @beta = qw(Beta);
my @chisquared = qw(ChiSquared);
my @gamma = qw(Gamma);
my @fisherf = qw(FisherF);
my @normal = qw(Normal Lognormal Levy);
my @stable = qw(Stable);
my @studentt = qw(StudentT);
my @int = qw(Geometric UniformInt);

my @nostd = qw(Arcsine Beta Laplace Levy Logistic Pareto Rayleigh Stable);
my @nomkl = qw(Arcsine Pareto Stable);

my %nostd;
my %nomkl;
$nostd{$_} = 1 for @nostd;
$nomkl{$_} = 1 for @nomkl;

my %dists = (
    inverse    => [@inverse],
    beta       => [@beta],
    chisquared => [@chisquared],
    gamma      => [@gamma],
    fisherf    => [@fisherf],
    normal     => [@normal],
    stable     => [@stable],
    studentt   => [@studentt],
    int        => [@int],
);

my @keys = qw(inverse beta chisquared gamma fisherf normal stable studentt
int);

my @dists;
for my $k (@keys) {
    my @val = @{$dists{$k}};
    for (@val) {
        push @dists, $_, if $_ =~ /$name/ or $k =~ /$name/ or $all;
    }
}

if ($run) {
    &run("llvm");
    &run("gnu");
    &run("intel");
    exit;
}

if ($build) {
    &build("llvm");
    &build("gnu");
    &build("intel");
    exit;
}

open my $texfile, '>', "random_distribution_$simd.tex";
say $texfile '\documentclass[';
say $texfile '  a4paper,';
say $texfile '  lines=42,';
say $texfile '  linespread=1.2,';
say $texfile '  fontsize=11pt,';
say $texfile '  fontset=Minion,';
say $texfile '  monofont=TheSansMonoCd,';
say $texfile '  monoscale=MatchLowercase,';
say $texfile ']{mbook}';
say $texfile '\input{../tex/macro}';
say $texfile '\pagestyle{empty}';
say $texfile '\begin{document}';
for my $k (@keys) {
    for (qw(llvm gnu intel)) {
        my $this_tex = "random_distribution";
        $this_tex .= "_\L$k";
        $this_tex .= "_$_";
        $this_tex .= "_$simd";
        &table($this_tex, &read($k, $_));
        say $texfile '\begin{table}';
        say $texfile "\\input{$this_tex}%";
        say $texfile "\\caption{\\textsc{$k ($_, sequential)}}";
        say $texfile '\end{table}';
        say $texfile '\begin{table}';
        say $texfile "\\input{${this_tex}_p}%";
        say $texfile "\\caption{\\textsc{$k ($_, parallel)}}";
        say $texfile '\end{table}';
    }
    say $texfile '\clearpage';
}
say $texfile '\end{document}';
close $texfile;
`lualatex -interaction=batchmode random_distribution_$simd.tex` if $pdf;

sub run
{
    my $dir = $build_dir{$_[0]};
    say $dir;
    my $txtfile;
    open $txtfile, '>',
    "random_distribution_$_[0]_$simd.txt" if $all and $write;
    my $header = 1;
    my @header;
    for my $dist (@dists) {
        my $cmd = "$make -C $dir random_distribution_perf_\L$dist-check 2>&1";
        my @lines = split "\n", `$cmd`;
        if ($header) {
            @header = grep { $_ =~ /Deterministics/ } @lines;
            if (@header) {
                say '=' x length($header[0]);
                say $header[0];
                say '-' x length($header[0]);
                $header = 0;
            }
        }
        my @result = grep { $_ =~ /Passed|Failed/ } @lines;
        say $_ for @result;
        say '-' x length($header[0]);
        if ($all and $write) {
            say $txtfile $_ for @result;
        }
    }
    close $txtfile if $all and $write;
}

sub build
{
    my $dir = $build_dir{$_[0]};
    say $dir;
    `$make -C $dir random_distribution_perf 2>&1`;
}

sub read
{
    my @val = @{$dists{$_[0]}};
    shift @_;
    open my $txtfile, '<', "random_distribution_$_[0]_$simd.txt";
    my @txt = grep {
    $_ =~ /(.*)<(double|u?int.._t)>.*(Passed|Failed).*/ } <$txtfile>;
    my $record;
    for (@txt) {
        my @this_record = split;
        my $name = shift @this_record;
        my $distname = $name;
        $distname =~ s/(.*)\(.*/$1/;
        $distname =~ s/(.*)<.*/$1/;
        $name =~ s/(.*)<double>(.*)/$1$2/;
        if (grep /^$distname$/, @val) {
            $record .= "$distname $name @this_record\n";
        }
    }
    $record;
}

sub table
{
    my $tex = shift @_;
    my @lines = split "\n", $_[0];

    my $wid = 0;
    my @name;
    my $index = 0;
    for (@lines) {
        my $name = (split)[1];
        $name =~ s/_/\\_/g;
        $name[$index] = '\texttt{' . $name . '}';
        if ($wid < length($name[-1])) {
            $wid = length($name[-1])
        }
        $index++;
    }

    my $header;
    $header .= '\tbfigures' . "\n";
    $header .= '\begin{tabularx}{\textwidth}{p{2in}RRRR}' . "\n";
    $header .= ' ' x 2 . '\toprule' . "\n";
    $header .= ' ' x 2 . 'Distribution & \std & \mckl & \batch & \mkl';
    $header .= " \\\\\n";
    $header .= ' ' x 2 . '\midrule' . "\n";

    my $footer;
    $footer .= ' ' x 2 . '\bottomrule' . "\n";
    $footer .= '\end{tabularx}' . "\n";
    $footer;

    my $table;
    my $table_p;
    $index = 0;
    for (@lines) {
        my ($distname, $name, $std, $mckl, $batch, $mkl, $mode) = split;
        if ($mode eq 'S') {
            $table .= ' ' x 2 . sprintf("%-${wid}s", $name[$index]);
            if ($nostd{$distname}) {
                $table .= ' & ' . sprintf('%-6s', '--');
            } else {
                $table .= &format($std);
            }
            $table .= &format($mckl);
            $table .= &format($batch);
            if ($nomkl{$distname}) {
                $table .= ' & ' . sprintf('%-6s', '--');
            } else {
                $table .= &format($mkl);
            }
            $table .= " \\\\\n";
        }
        if ($mode eq 'P') {
            $table_p .= ' ' x 2 . sprintf("%-${wid}s", $name[$index]);
            if ($nostd{$distname}) {
                $table_p .= ' & ' . sprintf('%-6s', '--');
            } else {
                $table_p .= &format($std);
            }
            $table_p .= &format($mckl);
            $table_p .= &format($batch);
            if ($nomkl{$distname}) {
                $table_p .= ' & ' . sprintf('%-6s', '--');
            } else {
                $table_p .= &format($mkl);
            }
            $table_p .= " \\\\\n";
        }
        $index++;
    }
    $table = $header . $table . $footer;
    $table_p = $header . $table_p . $footer;

    open my $texfile, '>', "$tex.tex";
    print $texfile $table;
    close $texfile;

    open my $texfile_p, '>', "${tex}_p.tex";
    print $texfile_p $table_p;
    close $texfile_p;

    $table;
}

sub format
{
    my $num = shift @_;
    if ($num > 100) {
        ' & ' . sprintf('%-6.0f', $num);
    } elsif ($num > 10) {
        ' & ' . sprintf('%-6.1f', $num);
    } else {
        ' & ' . sprintf('%-6.2f', $num);
    }
}
