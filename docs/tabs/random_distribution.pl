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

my $run = 0;
my $build = 0;
my $llvm = "../../build/release-llvm";
my $gnu = "../../build/release-gnu";
my $intel = "../../build/release-intel";
my $compiler = "llvm";
my $name;
my $scale = 3.8 / 2.6;
GetOptions(
    "run"        => \$run,
    "build"      => \$build,
    "llvm=s"     => \$llvm,
    "gnu=s"      => \$gnu,
    "intel=s"    => \$intel,
    "compiler=s" => \$compiler,
    "name=s"     => \$name,
    "scale"      => \$scale,
);
$build = 1 if $run;

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
    u01        => "Standard Uniform Distribution",
    inverse    => "Distributions using the Inverse Method",
    beta       => "Beta Distribution",
    chisquared => ":math:`\\chi^2`-Distribution",
    gamma      => "Gamma Distribution",
    fisherf    => "Fisher’s *F*-Distribution",
    normal     => "Normal and Related Distributions",
    stable     => "Stable Distribution",
    studentt   => "Student’s *t*-Distribution",
    int        => "Discrete Distribution",
);

my @distribution;
for my $k (@keys) {
    for my $r (@{$distribution{$k}}) {
        push @distribution, $r if $r =~ /$name/i or $k =~ /$name/i or !$name;
    }
}

my %compiler = (llvm => $llvm, gnu => $gnu, intel => $intel);
my @compiler = qw(llvm gnu intel);

my %nostd;
my %nomkl;
my @nostd = qw(U01CC U01OC U01OO Arcsine Beta Laplace Levy Logistic Pareto
Rayleigh Stable);
my @nomkl = qw(U01Canonical U01CC U01OC U01OO Arcsine Pareto Stable);
$nostd{$_} = 1 for @nostd;
$nomkl{$_} = 1 for @nomkl;

my %cpe_s;
my %cpe_m;
my %cpe_f;
my %cpe_l;
my %cpe_i;

&build;
&run;
&read;
&table;

sub build {
    return unless $build;

    my @target;
    for my $r (@distribution) {
        my $name = &distribution_name($r);
        push @target, "\Lrandom_distribution_${name}";
        push @target, "\Lrandom_distribution_${name}_novml";
    }

    if (@target) {
        for my $c (@compiler) {
            my $d = $compiler{$c};
            next if (not -d $d);
            say $d;
            if ($name) {
                `ninja -C $d @target 2>&1`;
            } else {
                `ninja -C $d random_distribution 2>&1`;
            }
        }
    }
}

sub run {
    return unless $run;

    for my $c (@compiler) {
        my $result;
        my $d = $compiler{$c};
        next if (not -d $d);
        say $d;
        for my $r (@distribution) {
            my @result;
            my $name = &distribution_name($r);
            my $cmd1 = "ninja -C $d";
            my $cmd2 = "ninja -C $d";
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
            "\Lrandom_distribution_${r}_${c}.txt";
            print $txt $_ for @result;
        }
    }
}

sub read {
    for my $r (@distribution) {
        open my $txt, "<", "random_distribution/" .
        "\Lrandom_distribution_${r}_${compiler}.txt";
        return unless $txt;
        my @lines = <$txt>;
        my @result = grep { /$r<(double|int64_t)>/ } @lines;
        for (@result) {
            my ($name, $cpe_s, $cpe_m, $cpe_v, $cpe_i, $lib) = (split);
            $cpe_s *= $scale;
            $cpe_m *= $scale;
            $cpe_v *= $scale;
            $cpe_i *= $scale;
            $name =~ s/(.*)<.*>(.*)/$1$2/;
            $cpe_s{$name} = 0xFFFF unless $cpe_s{$name};
            $cpe_m{$name} = 0xFFFF unless $cpe_m{$name};
            $cpe_f{$name} = 0xFFFF unless $cpe_f{$name};
            $cpe_l{$name} = 0xFFFF unless $cpe_l{$name};
            $cpe_i{$name} = 0xFFFF unless $cpe_i{$name};
            if ($lib eq "VMF") {
                $cpe_s{$name} = $cpe_s if $cpe_s < $cpe_s{$name};
                $cpe_m{$name} = $cpe_m if $cpe_m < $cpe_m{$name};
                $cpe_f{$name} = $cpe_v if $cpe_v < $cpe_f{$name};
                $cpe_i{$name} = $cpe_i if $cpe_i < $cpe_i{$name};
            }
            if ($lib eq "VML") {
                $cpe_s{$name} = $cpe_s if $cpe_s < $cpe_s{$name};
                $cpe_m{$name} = $cpe_m if $cpe_m < $cpe_m{$name};
                $cpe_l{$name} = $cpe_v if $cpe_v < $cpe_l{$name};
                $cpe_i{$name} = $cpe_i if $cpe_i < $cpe_i{$name};
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
        $table .= " " x 4 . ":header: Distribution, STD, MKL, VMF, VML, MKL\n\n";

        for my $r (@{$distribution{$k}}) {
            my @name = sort grep { /^$r(\(|$)/ } keys %cpe_s;
            for my $name (@name) {
                $table .= " " x 4 . sprintf("%-30s", "``$name``");
                $table .= " & ";
                if ($nostd{$r}) {
                    $table .= &format("—");
                } else {
                    $table .= &format($cpe_s{$name});
                }
                $table .= " & " . &format($cpe_m{$name});
                $table .= " & " . &format($cpe_f{$name});
                $table .= " & " . &format($cpe_l{$name});
                $table .= " & ";
                if ($nomkl{$r}) {
                    $table .= &format("—");
                } else {
                    $table .= &format($cpe_i{$name});
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

sub format
{
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

