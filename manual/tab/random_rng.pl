#!/usr/bin/env perl

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
my $build = 0;
my $llvm = "../../build/llvm-release";
my $gnu = "../../build/gnu-release";
my $intel = "../../build/intel-release";
my $make = "ninja";
my $name;
my $write = 0;
my $pdf = 0;
GetOptions(
    "run"      => \$run,
    "build"    => \$build,
    "llvm=s"   => \$llvm,
    "gnu=s"    => \$gnu,
    "intel=s"  => \$intel,
    "make=s"   => \$make,
    "name=s"   => \$name,
    "write"    => \$write,
    "pdf"      => \$pdf,
);
$build = 1 if $run;
$write = 0 if $name;

my @std = qw(mt19937 mt19937_64 minstd_rand0 minstd_rand ranlux24_base
ranlux48_base ranlux24 ranlux48 knuth_b);

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
    aes      => [@aes],
    philox   => [@philox],
    threefry => [@threefry],
    mkl      => [@mkl],
    rdrand   => [@rdrand],
);

my @keys = qw(std aes philox threefry mkl rdrand);

my @rng;
for my $k (@keys) {
    for my $r (@{$rng{$k}}) {
        push @rng, $r if $r =~ /$name/i or $k =~ /$name/i or !$name;
    }
}

my $cpuid = `cpuid_info`;
my $simd;
$simd = "sse2" if $cpuid =~ "SSE2";
$simd = "avx2" if $cpuid =~ "AVX2";
my @simd = qw(sse2 avx2);

my %compiler = (llvm => $llvm, gnu => $gnu, intel => $intel);
my @compiler = qw(llvm gnu intel);

my %cpb_s;
my %cpb_b;
my %cpb_sp;
my %cpb_bp;

&build;
&run;
&read;
&table;
&pdf;

sub build {
    return unless $build;

    my @target;
    push @target, "\Lrandom_rng_$_" for @rng;
    for my $c (@compiler) {
        my $d = $compiler{$c};
        say $d;
        if ($name) {
            `$make -C $d @target 2>&1`;
        } else {
            `$make -C $d random_rng 2>&1`;
        }
    }
}

sub run {
    return unless $run;

    for my $c (@compiler) {
        my @result;
        my $d = $compiler{$c};
        say $d;
        for my $r (@rng) {
            my $cmd = "$make -C $d \Lrandom_rng_$r-check 2>&1";
            my $cpb_s = 0xFFFF;
            my $cpb_b = 0xFFFF;
            my $cpb_sp = 0xFFFF;
            my $cpb_bp = 0xFFFF;
            my $count = 0;
            my $pass = 1;
            for (1..5) {
                my @lines = grep { $_ =~ /Passed|Failed/ } split "\n", `$cmd`;
                push @result, @lines;
                for (@lines) {
                    $count++;
                    $pass = 0 if (/Failed/);
                    my @cpb = (split)[3..6];
                    $cpb_s  = $cpb[0] if $cpb[0] < $cpb_s;
                    $cpb_b  = $cpb[1] if $cpb[1] < $cpb_b;
                    $cpb_sp = $cpb[2] if $cpb[2] < $cpb_sp;
                    $cpb_bp = $cpb[3] if $cpb[3] < $cpb_bp;
                }
            }
            my $line;
            if ($count) {
                $line .= sprintf("%-20s", $r);
                $line .= &format($cpb_s);
                $line .= &format($cpb_b);
                $line .= &format($cpb_sp);
                $line .= &format($cpb_bp);
                $line .= $pass ? "Passed" : "Failed";
                say $line;
            }
        }
        if ($write) {
            open my $txtfile, ">", "rng/random_rng_${c}_${simd}.txt";
            say $txtfile $_ for @result;
        }
    }
}

sub read {
    for my $c (@compiler) {
        for my $s (@simd) {
            open my $txtfile, "<", "rng/random_rng_${c}_${s}.txt";
            my @lines = <$txtfile>;
            for my $r (@rng) {
                my @result = grep { /$r\s+/ } @lines;
                if (@result) {
                    $cpb_s{$c}{$s}{$r} = 0xFFFF unless $cpb_s{$c}{$s}{$r};
                    $cpb_b{$c}{$s}{$r} = 0xFFFF unless $cpb_b{$c}{$s}{$r};
                    $cpb_sp{$c}{$s}{$r} = 0xFFFF unless $cpb_sp{$c}{$s}{$r};
                    $cpb_bp{$c}{$s}{$r} = 0xFFFF unless $cpb_bp{$c}{$s}{$r};
                    for (@result) {
                        my @cpb = (split)[3..6];
                        if ($cpb[0] < $cpb_s{$c}{$s}{$r}) {
                            $cpb_s{$c}{$s}{$r} = $cpb[0]
                        }
                        if ($cpb[1] < $cpb_b{$c}{$s}{$r}) {
                            $cpb_b{$c}{$s}{$r} = $cpb[1];
                        }
                        if ($cpb[2] < $cpb_sp{$c}{$s}{$r}) {
                            $cpb_sp{$c}{$s}{$r} = $cpb[2];
                        }
                        if ($cpb[3] < $cpb_bp{$c}{$s}{$r}) {
                            $cpb_bp{$c}{$s}{$r} = $cpb[3];
                        }
                    }
                }
            }
        }
    }
}

sub table {
    my $header;
    $header .= '\tbfigures' . "\n";
    $header .= '\begin{tabularx}{\textwidth}{p{1.5in}RRRRRR}' . "\n";
    $header .= " " x 2 . '\toprule' . "\n";
    $header .= " " x 2;
    $header .= '& \multicolumn{3}{c}{\single} ';
    $header .= '& \multicolumn{3}{c}{\batch}';
    $header .= " \\\\\n";
    $header .= " " x 2 . '\cmidrule(lr){2-4}\cmidrule(lr){5-7}' . "\n";
    $header .= " " x 2 . '\rng';
    $header .= ' & \llvm & \gnu & \intel';
    $header .= ' & \llvm & \gnu & \intel';
    $header .= " \\\\\n";
    $header .= " " x 2 . '\midrule' . "\n";

    my $footer;
    $footer .= " " x 2 . '\bottomrule' . "\n";
    $footer .= '\end{tabularx}' . "\n";

    for my $k (@keys) {
        for my $s (@simd) {
            my $table;
            my $table_p;
            for my $r (@{$rng{$k}}) {
                my $line_s;
                my $line_b;
                my $line_sp;
                my $line_bp;
                for my $c (@compiler) {
                    if ($cpb_s{$c}{$s}{$r}) {
                        $line_s .= " & ";
                        $line_s .= &format($cpb_s{$c}{$s}{$r});
                    }
                    if ($cpb_b{$c}{$s}{$r}) {
                        $line_b .= " & ";
                        $line_b .= &format($cpb_b{$c}{$s}{$r});
                    }
                    if ($cpb_sp{$c}{$s}{$r}) {
                        $line_sp .= " & ";
                        $line_sp .= &format($cpb_sp{$c}{$s}{$r});
                    }
                    if ($cpb_bp{$c}{$s}{$r}) {
                        $line_bp .= " & ";
                        $line_bp .= &format($cpb_bp{$c}{$s}{$r});
                    }
                }
                my $name = $r;
                $name =~ s/_/\\_/g;
                $name = " " x 2 . sprintf("%-30s", "\\texttt{$name}");
                if ($line_s and $line_b) {
                    $table .= $name . $line_s . $line_b . "\\\\\n";
                }
                if ($line_sp and $line_bp) {
                    $table_p .= $name . $line_sp . $line_bp . "\\\\\n";
                }
            }
            if ($table) {
                open my $texfile, ">", "\Lrandom_rng_${k}_${s}.tex";
                print $texfile $header;
                print $texfile $table;
                print $texfile $footer;
            }
            if ($table_p) {
                open my $texfile_p, ">", "\Lrandom_rng_${k}_${s}_p.tex";
                print $texfile_p $header;
                print $texfile_p $table_p;
                print $texfile_p $footer;
            }
        }
    }
}

sub pdf {
    return unless $pdf;

    open my $incfile, "<", "../tex/inc.tex";
    my @oldinc = <$incfile>;
    open my $incfile, ">", "../tex/inc.tex";
    say $incfile '\includeonly{tex/random_rng}';
    my $cmd;
    $cmd .= "cd ..;";
    $cmd .= " latexmk -f -silent";
    $cmd .= " -jobname=tab/random_rng";
    $cmd .= " manual.tex";
    `$cmd`;
    open my $incfile, ">", "../tex/inc.tex";
    print $incfile $_ for @oldinc;
}

sub format
{
    my $num = shift;
    if ($num eq "--") {
        sprintf("%-6s", $num);
    } elsif ($num > 100) {
        sprintf("%-6.0f", $num);
    } elsif ($num > 10) {
        sprintf("%-6.1f", $num);
    } else {
        sprintf("%-6.2f", $num);
    }
}
