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

my @keys = qw(std aesni philox threefry mkl rdrand);

my @rngs;
for my $k (@keys) {
    my @val = @{$rngs{$k}};
    for (@val) {
        push @rngs, $_, if $_ =~ /$name/ or $k =~ /$name/ or $all;
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

my $texfile;
if ($pdf) {
    open $texfile, '>', "random_rng_$simd.tex";
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
}
for (@keys) {
    my $this_tex = "random_rng";
    $this_tex .= "_\L$_";
    $this_tex .= "_$simd";
    if (&table($this_tex,
            &read($_, "llvm"),
            &read($_, "gnu"),
            &read($_, "intel"))) {
        if ($pdf) {
            say $texfile '\begin{table}';
            say $texfile "\\input{$this_tex}%";
            say $texfile "\\caption{\\textsc{$_ (sequential)}}";
            say $texfile '\end{table}';
            say $texfile '\begin{table}';
            say $texfile "\\input{${this_tex}_p}%";
            say $texfile "\\caption{\\textsc{$_ (parallel)}}";
            say $texfile '\end{table}';
        }
    }
}
if ($pdf) {
    say $texfile '\end{document}';
    close $texfile;
    `lualatex -interaction=batchmode random_rng_$simd.tex`;
}

sub run
{
    my $dir = $build_dir{$_[0]};
    say $dir;
    my $txtfile;
    open $txtfile, '>', "random_rng_$_[0]_$simd.txt" if $all and $write;
    my $header = 1;
    my @header;
    for my $rng (@rngs) {
        my $cmd = "$make -C $dir random_rng_\L$rng-check 2>&1";
        my @result;
        my $cpb = 0xFFFF;
        for (1..5) {
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
            my @this_result = grep { $_ =~ /Passed|Failed/ } @lines;
            if (@this_result) {
                for (@this_result) {
                    my $this_cpb = (split)[4];
                    if ($this_cpb < $cpb) {
                        $cpb = $this_cpb;
                        @result = @this_result;
                    }
                }
                last if $cpb > 10;
            }
        }
        say @result if @result;
        say $txtfile @result if @result and $all and $write;
    }
    say '-' x length($header[0]);
    close $txtfile if $all and $write;
}

sub build
{
    my $dir = $build_dir{$_[0]};
    say $dir;
    `$make -C $dir random_rng 2>&1`;
}

sub read
{
    my @val = @{$rngs{$_[0]}};
    shift @_;
    open my $txtfile, '<', "random_rng_$_[0]_$simd.txt";
    my @txt = grep { $_ =~ /Passed|Failed/ } <$txtfile>;
    my $record;
    for (@txt) {
        my @this_record = (split)[0, 3..6];
        my $name = $this_record[0];
        $record .= "@this_record\n" if (grep /^$name$/, @val);
    }
    $record;
}

sub table
{
    my $tex = shift @_;
    my @name;
    my @cpb1;
    my @cpb2;
    my @cpb1p;
    my @cpb2p;
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
            $cpb1p[$index] .= &format($record[3]);
            $cpb2p[$index] .= &format($record[4]);
            if ($wid < length($name[-1])) {
                $wid = length($name[-1]);
            }
            $index++;
        }
    }

    my $header;
    $header .= '\tbfigures' . "\n";
    $header .= '\begin{tabularx}{\textwidth}{p{1.5in}RRRRRR}' . "\n";
    $header .= ' ' x 2 . '\toprule' . "\n";
    $header .= ' ' x 2;
    $header .= '& \multicolumn{3}{c}{\single} ';
    $header .= '& \multicolumn{3}{c}{\batch}';
    $header .= " \\\\\n";
    $header .= ' ' x 2 . '\cmidrule(lr){2-4}\cmidrule(lr){5-7}' . "\n";
    $header .= ' ' x 2 . '\rng';
    $header .= ' & \llvm & \gnu & \intel';
    $header .= ' & \llvm & \gnu & \intel';
    $header .= " \\\\\n";
    $header .= ' ' x 2 . '\midrule' . "\n";

    my $footer;
    $footer .= ' ' x 2 . '\bottomrule' . "\n";
    $footer .= '\end{tabularx}' . "\n";
    $footer;

    my $table;
    my $table_p;
    if (@name) {
        my $index = 0;
        for (@name) {
            $table .= ' ' x 2;
            $table .= sprintf "%-${wid}s", $name[$index];
            $table .= $cpb1[$index];
            $table .= $cpb2[$index];
            $table .= " \\\\\n";

            $table_p .= ' ' x 2;
            $table_p .= sprintf "%-${wid}s", $name[$index];
            $table_p .= $cpb1p[$index];
            $table_p .= $cpb2p[$index];
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
    }
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
