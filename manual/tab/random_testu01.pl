#!/usr/bin/perl

# ============================================================================
#  MCKL/manual/tab/random_testu01.pl
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

my $failure = 1e-6;
my $suspect = 1e-3;
my $verbose = 0;
my $pdf = 0;

GetOptions(
    "failure=f" => \$failure,
    "suspect=f" => \$suspect,
    "verbose"   => \$verbose,
    "pdf"       => \$pdf,
);
$suspect = $failure if ($suspect < $failure);

my @battery = qw(SmallCrush Crush BigCrush);

my @u01 = qw(STD U01 U01CC U01CO U01OC U01OO);

my @std = qw(mt19937 mt19937_64 minstd_rand0 minstd_rand ranlux24_base
ranlux48_base ranlux24 ranlux48 knuth_b);

my @aesni = qw(AES128 AES128_64 AES192 AES192_64 AES256 AES256_64 ARS ARS_64);

my @philox = qw(Philox2x32 Philox2x32_64 Philox4x32 Philox4x32_64 Philox2x64
Philox2x64_64 Philox4x64 Philox4x64_64);

my @threefry = qw(Threefry2x32 Threefry2x32_64 Threefry4x32 Threefry4x32_64
Threefry2x64 Threefry2x64_64 Threefry4x64 Threefry4x64_64 Threefry8x64
Threefry8x64_64 Threefry16x64 Threefry16x64_64 Threefish256 Threefish256_64
Threefish512 Threefish512_64 Threefish1024 Threefish1024_64);

my @mkl = qw(MKL_ARS5 MKL_ARS5_64 MKL_PHILOX4X32X10 MKL_PHILOX4X32X10_64
MKL_MCG59 MKL_MCG59_64 MKL_MT19937 MKL_MT19937_64 MKL_MT2203 MKL_MT2203_64
MKL_SFMT19937 MKL_SFMT19937_64 MKL_NONDETERM MKL_NONDETERM_64);

my @rdrand = qw(RDRAND16 RDRAND32 RDRAND64);

my %rngs = (
    std      => [@std],
    aesni    => [@aesni],
    philox   => [@philox],
    threefry => [@threefry],
    mkl      => [@mkl],
    rdrand   => [@rdrand],
);

my @keys = qw(std aesni philox threefry mkl);

my %failure;
my %suspect;

for my $b (@battery) {
    for my $u (@u01) {
        for my $k (@keys) {
            &check($b, $_, $u) for @{$rngs{$k}};
        }
    }
}
&recheck();

for my $b (@battery) {
    for my $u (@u01) {
        for my $k (@keys) {
            &check($b, $_, $u, "_$u") for @{$rngs{$k}};
        }
    }
}
&recheck();

my $texfile;
if ($pdf) {
    open $texfile, '>', 'random_testu01.tex';
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
for my $b (@battery) {
    my $table = &table($b);
    if ($table) {
        my $this_tex = "random_testu01_\L$b";
        open my $this_texfile, '>', "$this_tex.tex";
        say $this_texfile $table if $table;
        if ($pdf) {
            say $texfile '\begin{table}';
            say $texfile "\\input{$this_tex}%";
            say $texfile "\\caption{$b}";
            say $texfile '\end{table}';
        }
    }
}
if ($pdf) {
    say $texfile '\end{document}';
    close $texfile;
    `lualatex -interaction=batchmode random_testu01.tex`;
}

sub check
{
    my $bat = shift;
    my $rng = shift;
    my $u01 = shift;
    my $suffix = shift;
    my $txt = "random_testu01_\L${bat}_${rng}${suffix}.txt";
    if (-e $txt) {
        open my $txtfile, '<', $txt;
        my @lines = <$txtfile>;
        s/\+$/\n/ for @lines;
        my $lines = "@lines";
        if ($lines =~ /$u01\n(.*?)tests were passed/s) {
            my $result = $1;
            if ($result =~ /--+\s*(.*?)\s*--+/s) {
                my @tests = split "\n", $1;
                my %this_failure;
                my %this_suspect;
                for (@tests) {
                    my ($num, $pval) = (split)[0, -1];
                    if ($pval < $failure or 1 - $pval < $failure) {
                        $this_failure{$num} = 1;
                    } elsif ($pval < $suspect or 1 - $pval < $suspect) {
                        $this_suspect{$num} = 1;
                    }
                }
                $failure{$bat}{$rng}{$u01}{$_} += 1 for (keys %this_failure);
                $suspect{$bat}{$rng}{$u01}{$_} += 1 for (keys %this_suspect);
            }
        }
    }
}

sub recheck
{
    for my $b (@battery) {
        open my $makefile, '>', "random_testu01_\L$b.make";
        say $makefile ".PHONY : all run\n";
        say $makefile "all :\n";
        my @target;
        for my $u (@u01) {
            for my $k (@keys) {
                for my $r (@{$rngs{$k}}) {
                    if ($suspect{$b}{$r}{$u}) {
                        my @num;
                        for (keys $suspect{$b}{$r}{$u}) {
                            push @num, $_;
                            if ($suspect{$b}{$r}{$u}{$_} > 1) {
                                $failure{$b}{$r}{$u}{$_} += 1;
                            }
                        }
                        if (@num) {
                            @num = sort @num;
                            my $cmd = "random_testu01_\L${b}_${r}";
                            push @target, "${cmd}_\L${u}.txt";
                            say $makefile "${cmd}_\L${u}.txt :";
                            print $makefile "\tninja -C ../.. ${cmd}\n";
                            print $makefile "\t./${cmd} $u stdout \\\n\t\t";
                            print $makefile "$_ " for @num;
                            print $makefile "\\\n";
                            print $makefile "\t\t>${cmd}_\L${u}.txt\n\n";
                        }
                    }
                }
            }
        }
        if (@target) {
            say $makefile "run : \\";
            say $makefile "\t$_ \\" for @target;
            say $makefile "\n";
            say $makefile ".PHONY : \\";
            say $makefile "\t$_ \\" for @target;
        }
        say $makefile "\n# vim:ft=make";
    }
}

sub table
{
    my $b = shift;

    my $header;
    $header .= '\begin{tabularx}{\textwidth}{p{1.5in}RRRRRR}' . "\n";
    $header .= ' ' x 2 . '\toprule' . "\n";
    $header .= ' ' x 2 . '\rng';
    $header .= ' & \std';
    $header .= ' & \textsc{u01}';
    $header .= ' & \textsc{u01cc}';
    $header .= ' & \textsc{u01co}';
    $header .= ' & \textsc{u01oc}';
    $header .= ' & \textsc{u01oo}';
    $header .= " \\\\\n";
    $header .= ' ' x 2 . '\midrule' . "\n";

    my $footer;
    $footer .= ' ' x 2 . '\bottomrule' . "\n";
    $footer .= '\end{tabularx}' . "\n";

    my $table;
    for my $k (@keys) {
        my @val = @{$rngs{$k}};
        for my $r (@val) {
            my $f = 0;
            for (@u01) {
                $f += 1 if $failure{$b}{$r}{$_};
            }
            if ($f) {
                $table .= ' ' x 2 . sprintf('%-20s', $r);
                for (@u01) {
                    my $num;
                    if ($failure{$b}{$r}{$_}) {
                        $num = ' & ' . keys $failure{$b}{$r}{$_};
                    } else {
                        $num .= ' & 0';
                    }
                    $table .= sprintf('%-6s', $num);
                }
                $table .= " \\\\\n";
            }
        }
    }
    $table = $header . $table . $footer if $table;
    $table;
}