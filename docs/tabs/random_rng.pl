#!/usr/bin/env perl

# ============================================================================
#  MCKL/docs/tabs/random_rng.pl
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

system "mkdir -p random_rng";

my $run = 0;
my $build = 0;
my $llvm = "../../build/release-llvm";
my $gnu = "../../build/release-gnu";
my $intel = "../../build/release-intel";
my $name;
my $scale = 3.8 / 2.6;
GetOptions(
    "run"      => \$run,
    "build"    => \$build,
    "llvm=s"   => \$llvm,
    "gnu=s"    => \$gnu,
    "intel=s"  => \$intel,
    "name=s"   => \$name,
    "scale"    => \$scale,
);
$build = 1 if $run;

my @std = qw(mt19937 mt19937_64 minstd_rand0 minstd_rand ranlux24_base
ranlux48_base ranlux24 ranlux48 knuth_b);

my @r123 = qw(R123_AESNI4x32 R123_ARS4x32 R123_Philox2x32 R123_Philox4x32
R123_Philox2x64 R123_Philox4x64 R123_Threefry2x32 R123_Threefry4x32
R123_Threefry2x64 R123_Threefry4x64);

my @aes = qw(AES128 AES192 AES256 ARS AES128_64 AES192_64 AES256_64 ARS_64);

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

my %rng = (
    std      => [@std],
    r123     => [@r123],
    aes      => [@aes],
    philox   => [@philox],
    threefry => [@threefry],
    mkl      => [@mkl],
    rdrand   => [@rdrand],
);

my @keys = qw(std r123 aes philox threefry mkl rdrand);

my %caption = (
    std      => "RNGs in the Standard Library",
    r123     => "RNGs in the Random123 Library",
    aes      => "``AESEngine``",
    philox   => "``PhiloxEngine``",
    threefry => "``ThreefryEngine``",
    mkl      => "``MKLEngine``",
    rdrand   => "Non-Determinstic RNGs",
);

my @rng;
for my $k (@keys) {
    for my $r (@{$rng{$k}}) {
        push @rng, $r if $r =~ /$name/i or $k =~ /$name/i or !$name;
    }
}

my %compiler = (llvm => $llvm, gnu => $gnu, intel => $intel);
my @compiler = qw(llvm gnu intel);

my %cpb_s;
my %cpb_b;

&build;
&run;
&read;
&table;

sub build {
    return unless $build;

    my @target;
    push @target, "\Lrandom_rng_$_" for @rng;
    if (@target) {
        for my $c (@compiler) {
            my $d = $compiler{$c};
            next if (not -d $d);
            say $d;
            if ($name) {
                `ninja -C $d @target 2>&1`;
            } else {
                `ninja -C $d random_rng 2>&1`;
            }
        }
    }
}

sub run {
    return unless $run;

    for my $c (@compiler) {
        my $d = $compiler{$c};
        next if (not -d $d);
        say $d;
        for my $r (@rng) {
            my $cmd = "ninja -C $d \Lrandom_rng_$r-check 2>&1";
            my $cpb_s = 0xFFFF;
            my $cpb_b = 0xFFFF;
            my $count = 0;
            my $pass = 1;
            my @result;
            for (1..5) {
                my @lines = `$cmd`;
                @lines = grep { /Passed|Failed/ } @lines;
                for (@lines) {
                    $count++;
                    $pass = 0 if /Failed/;
                    unless (/U01/) {
                        push @result, $_;
                        my @cpb = (split)[3, 4];
                        $cpb_s  = $cpb[0] if $cpb[0] < $cpb_s;
                        $cpb_b  = $cpb[1] if $cpb[1] < $cpb_b;
                    }
                }
            }
            my $line;
            if ($count) {
                $line .= sprintf("%-25s", $r);
                $line .= &format($cpb_s);
                $line .= &format($cpb_b);
                $line .= $pass ? "Passed" : "Failed";
                say $line;
            }
            if ($count) {
                open my $txt, ">",
                "\Lrandom_rng/random_rng_${r}_${c}.txt";
                print $txt $_ for @result;
            }
        }
    }
}

sub read {
    for my $c (@compiler) {
        for my $r (@rng) {
            open my $txt, "<",
            "\Lrandom_rng/random_rng_${r}_${c}.txt";
            return unless $txt;
            my @result = <$txt>;
            if (@result) {
                $cpb_s{$c}{$r} = 0xFFFF unless $cpb_s{$c}{$r};
                $cpb_b{$c}{$r} = 0xFFFF unless $cpb_b{$c}{$r};
                for (@result) {
                    my @cpb = (split)[3, 4];
                    $cpb[0] *= $scale;
                    $cpb[1] *= $scale;
                    if ($cpb[0] < $cpb_s{$c}{$r}) {
                        $cpb_s{$c}{$r} = $cpb[0]
                    }
                    if ($cpb[1] < $cpb_b{$c}{$r}) {
                        $cpb_b{$c}{$r} = $cpb[1];
                    }
                }
            }
        }
    }
}

sub table {
    my $table;
    for my $k (@keys) {
        my $caption  = "\n.. _tab-Performance of $caption{$k}:\n\n";
        $caption =~ s/``//g;
        $table .= $caption;
        $table .= ".. csv-table:: Performance of $caption{$k}\n";
        $table .= " " x 4 . ":delim: &\n";
        $table .= " " x 4 . ":header-rows: 1\n";
        $table .= " " x 4 . ":header: ,,Single,,,Batch,\n\n";
        $table .= " " x 4;
        $table .= sprintf("%-25s", "RNG");
        $table .= sprintf(" & %-6s", "LLVM");
        $table .= sprintf(" & %-6s", "GNU");
        $table .= sprintf(" & %-6s", "Intel");
        $table .= sprintf(" & %-6s", "LLVM");
        $table .= sprintf(" & %-6s", "GNU");
        $table .= sprintf(" & %-6s", "Intel");
        $table .= "\n";

        for my $r (@{$rng{$k}}) {
            my $line_s;
            my $line_b;
            for my $c (@compiler) {
                if ($cpb_s{$c}{$r}) {
                    $line_s .= " & ";
                    $line_s .= &format($cpb_s{$c}{$r});
                }
                if ($cpb_b{$c}{$r}) {
                    $line_b .= " & ";
                    $line_b .= &format($cpb_b{$c}{$r});
                }
            }
            my $name = $r;
            $name =~ s/R123_/r123::/g;
            $name = " " x 4 . sprintf("%-25s", "``$name``");
            if ($line_s and $line_b) {
                $table .= $name . $line_s . $line_b . "\n";
            }
        }
    }

    open my $rst, ">>", "../random_rng.rst";
    print $rst $table;
}

sub format
{
    my $num = shift;
    if ($num eq "—") {
        sprintf("%-6s", $num);
    } elsif ($num > 100) {
        sprintf("%-6.0f", $num);
    } elsif ($num > 10) {
        sprintf("%-6.1f", $num);
    } else {
        sprintf("%-6.2f", $num);
    }
}
