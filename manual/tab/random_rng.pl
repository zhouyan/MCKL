#!/usr/bin/perl

# ============================================================================
#  MCKL/manual/tab/random_rng.pl
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

my @std = qw(mt19937 mt19937_64 minstd_rand0 minstd_rand ranlux24_base
ranlux48_base ranlux24 ranlux48 knuth_b);

my @rngs = qw(STD Philox Threefry AES128 AES192 AES256 ARS RDRAND MKL);

my @clang_txt = &read('clang');
my @gcc_txt = &read('gcc');
my @intel_txt = &read('intel');

foreach (@rngs) {
    open my $texfile, '>', "random_rng_\L$_.tex";
    print $texfile &table(
        &filter($_, @clang_txt),
        &filter($_, @gcc_txt),
        &filter($_, @intel_txt));
}

sub read
{
    open my $txtfile, '<', "random_rng_$_[0].txt";
    my @txt = <$txtfile>;
    @txt
}

sub filter
{
    my $rng = shift @_;
    my $record;
    foreach (@_) {
        next if (!/Passed|Failed/);

        my ($name, $cpB1, $cpB2) = (split)[0, 5, 6];
        if ($rng eq 'STD') {
            next unless "@std" =~ /$name/;
        } elsif ($rng eq 'MKL') {
            next unless $name =~ /MKL/;
        } else {
            next unless $name =~ /$rng/;
            next if $name =~ /MKL/;
        }

        $record .= $name . ' ';
        $record .= $cpB1 . ' ';
        $record .= $cpB2 . "\n";
    }
    $record
}

sub table
{
    my @name;
    my @cpB1;
    my @cpB2;
    my $wid = 0;
    foreach (@_) {
        my @lines = split "\n", $_;
        my $index = 0;
        foreach (@lines) {
            my @record = split;
            $name[$index] = '\verb|' . $record[0] . '|';
            $cpB1[$index] .= &format($record[1]);
            $cpB2[$index] .= &format($record[2]);
            if ($wid < length($name[-1])) {
                $wid = length($name[-1]);
            }
            $index++;
        }
    }

    my $table;
    $table .= '\tbfigures' . "\n";
    $table .= '\begin{tabularx}{\textwidth}{p{1.25in}RRRRRR}' . "\n";
    $table .= ' ' x 2 . '\toprule' . "\n";
    $table .= ' ' x 2;
    $table .= '& \multicolumn{3}{c}{\textsc{loop}} ';
    $table .= '& \multicolumn{3}{c}{\textsc{batch}}';
    $table .= " \\\\\n";
    $table .= ' ' x 2 . '\cmidrule(lr){2-4}\cmidrule(lr){5-7}' . "\n";
    $table .= ' ' x 2 . '\rng';
    $table .= ' & \llvm & \gcc & \textsc{intel}';
    $table .= ' & \llvm & \gcc & \textsc{intel}';
    $table .= " \\\\\n";
    $table .= ' ' x 2 . '\midrule' . "\n";
    my $index = 0;
    foreach (@name) {
        $table .= ' ' x 2;
        $table .= sprintf "%-${wid}s", $name[$index];
        $table .= $cpB1[$index];
        $table .= $cpB2[$index];
        $table .= " \\\\\n";
        $index++;
    }
    $table .= ' ' x 2 . '\bottomrule' . "\n";
    $table .= '\end{tabularx}' . "\n";
    $table;
}
