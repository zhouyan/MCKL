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

my @llvm_txt = &read('llvm');
my @gnu_txt = &read('gnu');
my @intel_txt = &read('intel');

for (@rngs) {
    open my $texfile, '>', "random_rng_\L$_.tex";
    print $texfile &table(
        &filter($_, @llvm_txt),
        &filter($_, @gnu_txt),
        &filter($_, @intel_txt));
}

sub read
{
    open my $txtfile, '<', "random_rng_$_[0].txt";
    my @txt = grep { $_ =~ /Passed|Failed/ } <$txtfile>;
    @txt = sort @txt;
    open $txtfile, '>', "random_rng_$_[0].txt";
    print $txtfile @txt;
    @txt
}

sub filter
{
    my $rng = shift @_;
    my $record;
    for (@_) {
        my ($name, $size, $cpb1, $cpb2) = (split)[0, 1, 5, 6];
        if ($rng eq 'STD') {
            next unless "@std" =~ /$name/;
        } elsif ($rng eq 'MKL') {
            next unless $name =~ /MKL/;
        } elsif ($rng eq "Threefry") {
            next unless $name =~ /Threefry|Threefish/;
        } else {
            next unless $name =~ /$rng/;
            next if $name =~ /MKL/;
        }
        $size = (split /\//, $size)[1] if ($size =~ /\//);

        $record .= $name . ' ';
        $record .= $size . ' ';
        $record .= $cpb1 . ' ';
        $record .= $cpb2 . "\n";
    }
    $record
}

sub table
{
    my @name;
    my @size;
    my @cpb1;
    my @cpb2;
    my $wid = 0;
    for (@_) {
        my @lines = split "\n", $_;
        my $index = 0;
        for (@lines) {
            my @record = split;
            my $name = $record[0];
            my $size = $record[1];
            $name =~ s/_/\\_/g;
            $name[$index] = '\texttt{' . $name . '}';
            $size[$index] = $size;
            $cpb1[$index] .= &format($record[2]);
            $cpb2[$index] .= &format($record[3]);
            if ($wid < length($name[-1])) {
                $wid = length($name[-1]);
            }
            $index++;
        }
    }

    my $table;
    $table .= '\tbfigures' . "\n";
    $table .= '\begin{tabularx}{\textwidth}{p{1.5in}RRRRRRR}' . "\n";
    $table .= ' ' x 2 . '\toprule' . "\n";
    $table .= ' ' x 2;
    $table .= '& & \multicolumn{3}{c}{\single} ';
    $table .= '& \multicolumn{3}{c}{\batch}';
    $table .= " \\\\\n";
    $table .= ' ' x 2 . '\cmidrule(lr){3-5}\cmidrule(lr){6-8}' . "\n";
    $table .= ' ' x 2 . '\rng & Size';
    $table .= ' & \llvm & \gnu & \intel';
    $table .= ' & \llvm & \gnu & \intel';
    $table .= " \\\\\n";
    $table .= ' ' x 2 . '\midrule' . "\n";
    my $index = 0;
    for (@name) {
        $table .= ' ' x 2;
        $table .= sprintf "%-${wid}s", $name[$index];
        $table .= sprintf " & %-6s", $size[$index];
        $table .= $cpb1[$index];
        $table .= $cpb2[$index];
        $table .= " \\\\\n";
        $index++;
    }
    $table .= ' ' x 2 . '\bottomrule' . "\n";
    $table .= '\end{tabularx}' . "\n";
    $table;
}
