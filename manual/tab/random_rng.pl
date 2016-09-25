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
use Getopt::Long;

do 'format.pl';

my $run = 0;
my $simd;
GetOptions("run" => \$run, "simd=s" => \$simd);

if ($simd) {
    $run = 0;
} else {
    my $cpuid = `cpuid_info`;
    $simd = "sse2" if $cpuid =~ "SSE2";
    $simd = "avx2" if $cpuid =~ "AVX2";
}

my @std = qw(mt19937 mt19937_64 minstd_rand0 minstd_rand ranlux24_base
ranlux48_base ranlux24 ranlux48 knuth_b);

my @aesni = qw(AES128 AES192 AES256 ARS AES128_64 AES192_64 AES256_64 ARS_64);

my @philox = qw(Philox2x32 Philox4x32 Philox2x64 Philox4x64 Philox2x32_64
Philox4x32_64 Philox2x64_64 Philox4x64_64);

my @threefry = qw(Threefry2x32 Threefry4x32 Threefry2x64 Threefry4x64
Threefry8x64 Threefry16x64 Threefish256 Threefish512 Threefish1024
Threefry2x32_64 Threefry4x32_64 Threefry2x64_64 Threefry4x64_64 Threefry8x64_64
Threefry16x64_64 Threefish256_64 Threefish512_64 Threefish1024_64);

my @mkl = qw(MKL_ARS5 MKL_PHILOX4X32X10 MKL_MCG59 MKL_MT19937 MKL_MT2203
MKL_SFMT19937 MKL_NONDETERM MKL_ARS5_64 MKL_PHILOX4X32X10_64 MKL_MCG59_64
MKL_MT19937_64 MKL_MT2203_64 MKL_SFMT19937_64 MKL_NONDETERM_64);

my @rdrand = qw(RDRAND16 RDRAND32 RDRAND64);

my %rngs = (
    std      => [@std],
    aesni    => [@aesni],
    philox   => [@philox],
    threefry => [@threefry],
    mkl      => [@mkl],
    rdrand   => [@rdrand],
);

if ($run) {
    &run("llvm");
    &run("gnu");
    &run("intel");
    exit;
}

for (keys %rngs) {
    my $tex = "random_rng";
    $tex .= "_\L$_";
    $tex .= "_$simd";
    $tex .= ".tex";
    open my $texfile, '>', $tex;
    print $texfile &table(
        &read($_, "llvm"),
        &read($_, "gnu"),
        &read($_, "intel"));
}

sub run
{
    my $dir = "../../build/$_[0]-release-sys";
    open my $txtfile, '>', "random_rng_$_[0]_$simd.txt";
    for (sort(keys %rngs)) {
        my @val = @{$rngs{$_}};
        for my $rng (@val) {
            my $cmd = "ninja -C $dir random_rng_\L$rng-check 2>&1";
            my @lines = split "\n", `$cmd`;
            my @result = grep { $_ =~ /Passed|Failed/ } @lines;
            say @result if @result;
            say $txtfile @result if @result;
        }
    }
    close $txtfile;
}

sub read
{
    my @val = @{$rngs{$_[0]}};
    shift @_;
    open my $txtfile, '<', "random_rng_$_[0]_$simd.txt";
    my @txt = grep { $_ =~ /Passed|Failed/ } <$txtfile>;
    my $record;
    for (@txt) {
        my ($name, $cpb1, $cpb2) = (split)[0, 3, 4];
        $record .= "$name $cpb1 $cpb2\n" if (grep /^$name$/, @val);
    }
    $record;
}

sub table
{
    my @name;
    my @cpb1;
    my @cpb2;
    my $wid = 0;
    for (@_) {
        my @lines = split "\n", $_;
        my $index = 0;
        for (@lines) {
            my @record = split;
            my $name = $record[0];
            $name =~ s/_/\\_/g;
            $name[$index] = '\texttt{' . $name . '}';
            $cpb1[$index] .= &format($record[1]);
            $cpb2[$index] .= &format($record[2]);
            if ($wid < length($name[-1])) {
                $wid = length($name[-1]);
            }
            $index++;
        }
    }

    my $table;
    $table .= '\tbfigures' . "\n";
    $table .= '\begin{tabularx}{\textwidth}{p{1.5in}RRRRRR}' . "\n";
    $table .= ' ' x 2 . '\toprule' . "\n";
    $table .= ' ' x 2;
    $table .= '& \multicolumn{3}{c}{\single} ';
    $table .= '& \multicolumn{3}{c}{\batch}';
    $table .= " \\\\\n";
    $table .= ' ' x 2 . '\cmidrule(lr){2-4}\cmidrule(lr){5-7}' . "\n";
    $table .= ' ' x 2 . '\rng';
    $table .= ' & \llvm & \gnu & \intel';
    $table .= ' & \llvm & \gnu & \intel';
    $table .= " \\\\\n";
    $table .= ' ' x 2 . '\midrule' . "\n";
    my $index = 0;
    for (@name) {
        $table .= ' ' x 2;
        $table .= sprintf "%-${wid}s", $name[$index];
        $table .= $cpb1[$index];
        $table .= $cpb2[$index];
        $table .= " \\\\\n";
        $index++;
    }
    $table .= ' ' x 2 . '\bottomrule' . "\n";
    $table .= '\end{tabularx}' . "\n";
    $table;
}
