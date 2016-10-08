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
my $compiler = "llvm";
my $make = "ninja";
my $name;
my $write = 0;
GetOptions(
    "run"        => \$run,
    "pdf"        => \$pdf,
    "build"      => \$build,
    "simd=s"     => \$simd,
    "llvm=s"     => \$llvm,
    "gnu=s"      => \$gnu,
    "intel=s"    => \$intel,
    "compiler=s" => \$compiler,
    "make=s"     => \$make,
    "name=s"     => \$name,
    "write"      => \$write,
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

if ($run or $build) {
    &build("llvm");
    &build("gnu");
    &build("intel");
    exit if (not $run);
}

if ($run) {
    &run("llvm");
    &run("gnu");
    &run("intel");
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
    my $this_tex = "random_distribution_\L$k";
    `cp ${this_tex}_${compiler}_${simd}.tex   ${this_tex}_${simd}.tex`;
    `cp ${this_tex}_${compiler}_${simd}_p.tex ${this_tex}_${simd}_p.tex`;
    say $texfile '\clearpage';
}
say $texfile '\end{document}';
close $texfile;
`lualatex -interaction=batchmode random_distribution_$simd.tex` if $pdf;

sub build
{
    my $dir = $build_dir{$_[0]};
    say $dir;
    if ($all) {
        `$make -C $dir random_distribution_perf 2>&1`;
    } else {
        my @target;
        push @target, "random_distribution_perf_\L$_" for (@dists);
        push @target, "random_distribution_perf_\L${_}_novml" for (@dists);
        `$make -C $dir @target`;
    }
}

sub run
{
    my $dir = $build_dir{$_[0]};
    say $dir;
    my $txtfile;
    open $txtfile, '>',
    "distribution/random_distribution_$_[0]_$simd.txt" if $all and $write;
    my $header = 1;
    my @header;
    for my $dist (@dists) {
        my $cmd1 =
        "$make -C $dir random_distribution_perf_\L$dist-check 2>&1";
        my $cmd2 =
        "$make -C $dir random_distribution_perf_\L${dist}_novml-check 2>&1";
        my $out;
        $out .= `$cmd1`;
        $out .= `$cmd2`;
        my @lines = split "\n", $out;
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

sub read
{
    my @val = @{$dists{$_[0]}};
    shift @_;
    open my $txtfile, '<', "distribution/random_distribution_$_[0]_$simd.txt";
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
    my @scpp = grep { /SCPP/ } @lines;
    my @pcpp = grep { /PCPP/ } @lines;
    my @svml = grep { /SVML/ } @lines;
    my @pvml = grep { /PVML/ } @lines;

    my $wid = 0;
    my @dist;
    my @name;
    my @std_s;
    my @std_p;
    my @mckl_s;
    my @mckl_p;
    my @batch_scpp;
    my @batch_svml;
    my @batch_pcpp;
    my @batch_pvml;
    my @mkl_s;
    my @mkl_p;
    my $index = 0;
    for (@scpp) {
        my $dist;
        my $name;
        my $std;
        my $mckl;
        my $batch;
        my $mkl;
        my $std_s = 0xFFF;
        my $std_p = 0xFFF;
        my $mckl_s = 0xFFF;
        my $mckl_p = 0xFFF;
        my $batch_scpp = 0xFFF;
        my $batch_svml = 0xFFF;
        my $batch_pcpp = 0xFFF;
        my $batch_pvml = 0xFFF;
        my $mkl_s = 0xFFF;
        my $mkl_p = 0xFFF;

        ($dist, $name, $std, $mckl, $batch, $mkl) = split ' ', $scpp[$index];
        $std_s      = $std  if $std  < $std_s;
        $mckl_s     = $mckl if $mckl < $mckl_s;
        $batch_scpp = $batch;
        $mkl_s      = $mkl  if $mkl  < $mkl_s;

        ($dist, $name, $std, $mckl, $batch, $mkl) = split ' ', $pcpp[$index];
        $std_p      = $std  if $std  < $std_p;
        $mckl_p     = $mckl if $mckl < $mckl_p;
        $batch_pcpp = $batch;
        $mkl_p      = $mkl  if $mkl  < $mkl_p;

        ($dist, $name, $std, $mckl, $batch, $mkl) = split ' ', $svml[$index];
        $std_s      = $std  if $std  < $std_s;
        $mckl_s     = $mckl if $mckl < $mckl_s;
        $batch_svml = $batch;
        $mkl_s      = $mkl  if $mkl  < $mkl_s;

        ($dist, $name, $std, $mckl, $batch, $mkl) = split ' ', $pvml[$index];
        $std_p      = $std  if $std  < $std_p;
        $mckl_p     = $mckl if $mckl < $mckl_p;
        $batch_pvml = $batch;
        $mkl_p      = $mkl  if $mkl  < $mkl_p;

        $name =~ s/_/\\_/g;
        $name = '\texttt{' . $name . '}';
        if ($wid < length($name[-1])) {
            $wid = length($name[-1])
        }

        push @dist,       $dist;
        push @name,       $name;
        push @std_s,      $std_s;
        push @std_p,      $std_p;
        push @mckl_s,     $mckl_s;
        push @mckl_p,     $mckl_p;
        push @batch_scpp, $batch_scpp;
        push @batch_svml, $batch_svml;
        push @batch_pcpp, $batch_pcpp;
        push @batch_pvml, $batch_pvml;
        push @mkl_s,      $mkl_s;
        push @mkl_p,      $mkl_p;

        $index++;
    }

    my $header;
    $header .= '\tbfigures' . "\n";
    $header .= '\begin{tabularx}{\textwidth}{p{2in}RRRRR}' . "\n";
    $header .= ' ' x 2 . '\toprule' . "\n";
    $header .= ' ' x 2 . 'Distribution & \std & \mckl & \batch & \vml & \mkl';
    $header .= " \\\\\n";
    $header .= ' ' x 2 . '\midrule' . "\n";

    my $footer;
    $footer .= ' ' x 2 . '\bottomrule' . "\n";
    $footer .= '\end{tabularx}' . "\n";
    $footer;

    my $table;
    my $table_p;
    $index = 0;
    for (@name) {
        $table .= ' ' x 2 . sprintf("%-${wid}s", $name[$index]);
        $table_p .= ' ' x 2 . sprintf("%-${wid}s", $name[$index]);
        if ($nostd{$dist[$index]}) {
            $table .= ' & ' . sprintf('%-6s', '--');
            $table_p .= ' & ' . sprintf('%-6s', '--');
        } else {
            $table .= &format($std_s[$index]);
            $table_p .= &format($std_p[$index]);
        }
        $table .= &format($mckl_s[$index]);
        $table .= &format($batch_scpp[$index]);
        $table .= &format($batch_svml[$index]);
        $table_p .= &format($mckl_p[$index]);
        $table_p .= &format($batch_pcpp[$index]);
        $table_p .= &format($batch_pvml[$index]);
        if ($nomkl{$dist[$index]}) {
            $table .= ' & ' . sprintf('%-6s', '--');
            $table_p .= ' & ' . sprintf('%-6s', '--');
        } else {
            $table .= &format($mkl_s[$index]);
            $table_p .= &format($mkl_p[$index]);
        }
        $table .= " \\\\\n";
        $table_p .= " \\\\\n";
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
