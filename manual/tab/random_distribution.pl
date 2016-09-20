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

do 'format.pl';

my @inverse = qw(Arcsine Cauchy Exponential ExtremeValue Laplace Logistic
Pareto Rayleigh UniformReal Weibull);

my @int = qw(Geometric UniformInt);

my @normal = qw(Normal Lognormal Levy);

my @nostd = qw(Arcsine Beta Laplace Levy Logistic Pareto Rayleigh Stable);

my @nomkl = qw(Arcsine Pareto Stable);

my %distribution;
my %cpe;
my $txt;
open my $txtfile, '<', 'random_distribution.txt';
while (<$txtfile>) {
    if (/<(double|int)>\(.*(Passed|Failed)/) {
        $txt .= $_;
        my @record = split;
        my $name = shift @record;
        $name =~ s/(.*)<double>(.*)/$1$2/;
        $name =~ s/_/\\_/g;
        $name = '\texttt{' . $name . "}\n";
        my $basename = $1;
        my $cpe;
        if ("@nostd" =~ /$basename/) {
            shift @record;
            $cpe .= sprintf ' & %-4s', '--';
        } else {
            $cpe .= &format(shift @record);
        }
        $cpe .= &format(shift @record);
        $cpe .= &format(shift @record);
        if ("@nomkl" =~ /$basename/) {
            shift @record;
            $cpe .= sprintf ' & %-4s', '--';
        } else {
            $cpe .= &format(shift @record);
        }
        $cpe .= "\n";
        if ("@inverse" =~ /$basename/) {
            $distribution{'inverse'} .= $name;
            $cpe{'inverse'} .= $cpe;
        } elsif ("@int" =~ /$basename/) {
            $distribution{'int'} .= $name;
            $cpe{'int'} .= $cpe;
        } elsif ("@normal" =~ /$basename/) {
            $distribution{'normal'} .= $name;
            $cpe{'normal'} .= $cpe;
        } else {
            $distribution{$basename} .= $name;
            $cpe{$basename} .= $cpe;
        }
    }
}
open $txtfile, '>', 'random_distribution.txt';
print $txtfile $txt;

while (my ($basename, $name) = each %distribution) {
    my @dist = split "\n", $distribution{$basename};
    my @cpe = split "\n", $cpe{$basename};
    my $wid = 0;
    for (@dist) {
        if ($wid < length($_)) {
            $wid = length($_);
        }
    }

    my $table;
    $table .= '\tbfigures' . "\n";
    $table .= '\begin{tabularx}{\textwidth}{p{2in}RRRR}' . "\n";
    $table .= ' ' x 2 . '\toprule' . "\n";
    $table .= ' ' x 2;
    $table .= 'Distribution & \std & \mckl & \batch & \mkl';
    $table .= " \\\\\n";
    $table .= ' ' x 2 . '\midrule' . "\n";
    my $index = 0;
    for (@dist) {
        $table .= ' ' x 2;
        $table .= sprintf "%-${wid}s", $dist[$index];
        $table .= $cpe[$index];
        $table .= " \\\\\n";
        $index++;
    }
    $table .= ' ' x 2 . '\bottomrule' . "\n";
    $table .= '\end{tabularx}' . "\n";
    open my $texfile, '>', "random_distribution_\L$basename.tex";
    print $texfile $table;
}
