#!/usr/bin/env perl

# ============================================================================
#  MCKL/docs/tabs/random_distribution.pl
# ----------------------------------------------------------------------------
#  MCKL: Monte Carlo Kernel Library
# ----------------------------------------------------------------------------
#  Copyright (c) 2013-2017, Yan Zhou
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
use open ":std", ":encoding(UTF-8)";
use utf8;

system "mkdir -p random_distribution";

my $llvm = "../../build/release-llvm";
my $gnu = "../../build/release-gnu";
my $intel = "../../build/release-intel";
my $compiler = "llvm";
my $scale = 3.8 / 2.6;
GetOptions(
    "llvm=s"     => \$llvm,
    "gnu=s"      => \$gnu,
    "intel=s"    => \$intel,
    "compiler=s" => \$compiler,
    "scale"      => \$scale,
);

my @u01 = qw(U01Canonical U01CC U01CO U01OC U01OO);
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

my %distribution = (
    u01        => [@u01],
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

my @keys = qw(u01 inverse beta chisquared gamma fisherf normal stable studentt
int);

my %caption = (
    u01        => "Standard Uniform Distributions",
    inverse    => "Distributions using the Inverse Method",
    beta       => "Beta Distribution",
    chisquared => ":math:`\\chi^2`-Distribution",
    gamma      => "Gamma Distribution",
    fisherf    => "Fisher’s *F*-Distribution",
    normal     => "Normal and Related Distributions",
    stable     => "Stable Distribution",
    studentt   => "Student’s *t*-Distribution",
    int        => "Discrete Distributions",
);

my @distribution;
for my $k (@keys) {
    for my $d (@{$distribution{$k}}) {
        push @distribution, $d;
    }
}

my %compiler = (llvm => $llvm, gnu => $gnu, intel => $intel);

my %nostd;
my %nomkl;
my @nostd = qw(U01CC U01OC U01OO Arcsine Beta Laplace Levy Logistic Pareto
Rayleigh Stable);
my @nomkl = qw(U01Canonical U01CC U01OC U01OO Arcsine Pareto Stable);
$nostd{$_} = 1 for @nostd;
$nomkl{$_} = 1 for @nomkl;

my %cpe_std;
my %cpe_mckl;
my %cpe_vmf;
my %cpe_vml;
my %cpe_mkl;

&build;
&run;
&read;
&table;

sub build {
    my $dir = $compiler{$compiler};
    next unless -d $dir;
    say $dir;
    `ninja -C $dir random_distribution 2>&1`;
}

sub run {
    my $result;
    my $dir = $compiler{$compiler};
    next unless -d $dir;
    say $dir;
    for my $d (@distribution) {
        my @result;
        my $name = &distribution_name($d);
        my $cmd1 = "ninja -C $dir";
        my $cmd2 = "ninja -C $dir";
        $cmd1 .= " \Lrandom_distribution_${name}-check 2>&1";
        $cmd2 .= " \Lrandom_distribution_${name}_novml-check 2>&1";
        my @lines1 = `$cmd1`;
        my @lines2 = `$cmd2`;
        @lines1 = grep { /Passed|Failed/ } @lines1;
        @lines2 = grep { /Passed|Failed/ } @lines2;
        push @result, @lines1;
        push @result, @lines2;
        for (@lines1) {
            print $_ if /<(double|int64_t)>.*VML/;
        }
        for (@lines2) {
            print $_ if /<(double|int64_t)>.*VML/;
        }
        open my $txt, ">", "random_distribution/" .
        "\Lrandom_distribution_${d}.txt";
        print $txt $_ for @result;
    }
}

sub read {
    for my $d (@distribution) {
        open my $txt, "<", "random_distribution/" .
        "\Lrandom_distribution_${d}.txt";
        return unless $txt;
        my @lines = <$txt>;
        my @result = grep { /$d<(double|int64_t)>/ } @lines;
        for (@result) {
            my ($name, $cpe_std, $cpe_mckl, $cpe_vm, $cpe_mkl, $lib) = (split);
            $cpe_std  *= $scale;
            $cpe_mckl *= $scale;
            $cpe_vm   *= $scale;
            $cpe_mkl  *= $scale;
            $name =~ s/(.*)<.*>(.*)/$1$2/;
            $cpe_std{$name}  = 0xFFFF unless $cpe_std{$name};
            $cpe_mckl{$name} = 0xFFFF unless $cpe_mckl{$name};
            $cpe_vmf{$name}  = 0xFFFF unless $cpe_vmf{$name};
            $cpe_vml{$name}  = 0xFFFF unless $cpe_vml{$name};
            $cpe_mkl{$name}  = 0xFFFF unless $cpe_mkl{$name};
            if ($lib eq "VMF") {
                $cpe_std{$name}  = $cpe_std  if $cpe_std  < $cpe_std{$name};
                $cpe_mckl{$name} = $cpe_mckl if $cpe_mckl < $cpe_mckl{$name};
                $cpe_vmf{$name}  = $cpe_vm   if $cpe_vm   < $cpe_vmf{$name};
                $cpe_mkl{$name}  = $cpe_mkl  if $cpe_mkl  < $cpe_mkl{$name};
            }
            if ($lib eq "VML") {
                $cpe_std{$name}  = $cpe_std  if $cpe_std  < $cpe_std{$name};
                $cpe_mckl{$name} = $cpe_mckl if $cpe_mckl < $cpe_mckl{$name};
                $cpe_vml{$name}  = $cpe_vm   if $cpe_vm   < $cpe_vml{$name};
                $cpe_mkl{$name}  = $cpe_mkl  if $cpe_mkl  < $cpe_mkl{$name};
            }
        }
    }
}

sub table {
    my $table;
    for my $k (@keys) {
        my $caption  = "\n.. _tab-Performance of $caption{$k}:\n\n";
        $caption =~ s/:math:`\\chi\^2`/chi-Squared/g;
        $caption =~ s/\*(.*)\*/$1/g;
        $table .= $caption;
        $table .= ".. csv-table:: Performance of $caption{$k}\n";
        $table .= " " x 4 . ":delim: &\n";
        $table .= " " x 4 . ":header: Distribution, STD, MCKL, VMF, VML, MKL\n\n";

        for my $d (@{$distribution{$k}}) {
            my @name = sort grep { /^$d(\(|$)/ } keys %cpe_mckl;
            for my $name (@name) {
                $table .= " " x 4 . sprintf("%-30s", "``$name``");
                $table .= " & ";
                if ($nostd{$d}) {
                    $table .= &format("—");
                } else {
                    $table .= &format($cpe_std{$name});
                }
                $table .= " & " . &format($cpe_mckl{$name});
                $table .= " & " . &format($cpe_vmf{$name});
                $table .= " & " . &format($cpe_vml{$name});
                $table .= " & ";
                if ($nomkl{$d}) {
                    $table .= &format("—");
                } else {
                    $table .= &format($cpe_mkl{$name});
                }
                $table .= "\n";
            }
        }
    }
    $table =~ s/  *$//gm;

    open my $rst, "<", "../random_distribution.rst";
    my @lines;
    for (<$rst>) {
        last if /^\.\. _tab-/;
        push @lines, $_;
    }
    pop @lines;

    open my $rst, ">", "../random_distribution.rst";
    print $rst $_ for @lines;
    print $rst $table;
}

sub distribution_name {
    my $name = shift;
    if ($name eq "U01Canonical") {
        $name = "U01_Canonical";
    } elsif ($name =~ /U01(..)/) {
        $name =~ s/U01(..)/U01_$1/;
    } else {
        $name =~ s/([A-Z])/_$1/g;
        $name =~ s/^_//g;
    }
    $name
}

sub format {
    my $num = shift;
    if ($num eq "—") {
        sprintf("%-6s", $num);
    } elsif ($num > 100) {
        sprintf('%-6.0f', $num);
    } elsif ($num > 10) {
        sprintf('%-6.1f', $num);
    } else {
        sprintf('%-6.2f', $num);
    }
}

