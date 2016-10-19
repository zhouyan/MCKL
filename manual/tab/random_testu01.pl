#!/usr/bin/env perl

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
use Data::Dumper;

my $failure = 1e-6;
my $suspect = 1e-3;
my $pdf = 0;
GetOptions(
    "failure=f" => \$failure,
    "suspect=f" => \$suspect,
    "pdf"       => \$pdf,
);
$suspect = sqrt($failure) if ($suspect < $failure);

my @bat = qw(SmallCrush Crush BigCrush);

my @u01 = qw(STD U01 U01CC U01CO U01OC U01OO);

my @rng = qw(
mt19937 mt19937_64
minstd_rand0 minstd_rand
ranlux24_base ranlux48_base
ranlux24 ranlux48
knuth_b
AES128 AES128_64
AES192 AES192_64
AES256 AES256_64
ARS ARS_64
Philox2x32 Philox2x32_64
Philox4x32 Philox4x32_64
Philox2x64 Philox2x64_64
Philox4x64 Philox4x64_64
Threefry2x32 Threefry2x32_64
Threefry4x32 Threefry4x32_64
Threefry2x64 Threefry2x64_64
Threefry4x64 Threefry4x64_64
Threefry8x64 Threefry8x64_64
Threefry16x64 Threefry16x64_64
Threefish256 Threefish256_64
Threefish512 Threefish512_64
Threefish1024 Threefish1024_64
MKL_ARS5 MKL_ARS5_64
MKL_PHILOX4X32X10 MKL_PHILOX4X32X10_64
MKL_MCG59 MKL_MCG59_64
MKL_MT19937 MKL_MT19937_64
MKL_MT2203 MKL_MT2203_64
MKL_SFMT19937 MKL_SFMT19937_64
MKL_NONDETERM MKL_NONDETERM_64
RDRAND16 RDRAND32 RDRAND64);

my $subdir;
my %failure;
my %suspect;

my @subdirs = qw(parallel sqeuntial);
for (@subdirs) {
    $subdir = $_;
    %failure = ();
    %suspect = ();
    &filter;
    &check;
    &target;
    &recheck;
    &count;
    &table;
    &pdf;
}

sub filter {
    for my $b (@bat) {
        for my $r (@rng) {
            &filter_txt("${b}_${r}");
            for my $u (@u01) {
                &filter_txt("${b}_${r}_${u}");
            }
        }
    }
}

sub check {
    for my $b (@bat) {
        for my $r (@rng) {
            for my $u (@u01) {
                &check_txt("${b}_${r}", $b, $r, $u);
            }
        }
    }
}

sub target {
    for my $b (keys %suspect) {
        my %target;
        for my $r (keys %{$suspect{$b}}) {
            for my $u (sort keys %{$suspect{$b}{$r}}) {
                my @keys = sort keys %{$suspect{$b}{$r}{$u}};
                my $bin = "\Lrandom_testu01_${b}_${r}";
                $target{$bin} .= "\t./$bin $u";
                $target{$bin} .= " $_" for @keys;
                $target{$bin} .= "\n";
            }
        }
        if (%target) {
            my @keys = sort keys %target;
            open my $makefile, ">", "\Ltestu01/$subdir/random_testu01_$b.make";
            print $makefile ".PHONY : all run";
            print $makefile " \\\n\t$_" for @keys;
            print $makefile "\n";
            print $makefile "all :\n\n";
            print $makefile "run :";
            print $makefile " \\\n\t$_" for @keys;
            print $makefile "\n\n";
            for (@keys) {
                print $makefile "$_ :\n";
                print $makefile "\tninja -C ../.. $_\n";
                print $makefile $target{$_};
                print $makefile "\n";
            }
            print $makefile "# vim:ft=make\n";
        }
    }
}

sub recheck {
    for my $b (@bat) {
        for my $r (@rng) {
            for my $u (@u01) {
                &check_txt("${b}_${r}_${u}", $b, $r, $u);
            }
        }
    }
}

sub count {
    for my $b (keys %suspect) {
        for my $r (keys %{$suspect{$b}}) {
            for my $u (keys %{$suspect{$b}{$r}}) {
                for my $n (keys %{$suspect{$b}{$r}{$u}}) {
                    if ($suspect{$b}{$r}{$u}{$n} > 1) {
                        $failure{$b}{$r}{$u}{$n} = 1;
                    }
                }
            }
        }
    }
}

sub table {
    my $header;
    $header .= '\begin{tabularx}{\textwidth}{p{1.5in}RRRRRR}' . "\n";
    $header .= " " x 2 . '\toprule' . "\n";
    $header .= " " x 2 . '\rng';
    $header .= ' & \std';
    $header .= ' & \textsc{u01}';
    $header .= ' & \textsc{u01cc}';
    $header .= ' & \textsc{u01co}';
    $header .= ' & \textsc{u01oc}';
    $header .= ' & \textsc{u01oo}';
    $header .= " \\\\\n";
    $header .= " " x 2 . '\midrule' . "\n";

    my $footer;
    $footer .= " " x 2 . '\bottomrule' . "\n";
    $footer .= '\end{tabularx}' . "\n";

    for my $b (keys %failure) {
        my $table;
        for my $r (@rng) {
            if ($failure{$b}{$r}) {
                my $name = $r;
                $name =~ s/_/\\_/g;
                $table .= " " x 2 . sprintf("%-30s", "\\texttt{$name}");
                for my $u (@u01)  {
                    $table .= " & ";
                    if ($failure{$b}{$r}{$u}) {
                        my $num = keys %{$failure{$b}{$r}{$u}};
                        $table .= sprintf("%-6s", $num);
                    } else {
                        $table .= sprintf("%-6s", 0);
                    }
                }
                $table .= " \\\\\n"
            }
        }
        my $suffix;
        $suffix = "_p" if $subdir eq "parallel";
        open my $texfile, ">", "\Lrandom_testu01_$b$suffix.tex";
        print $texfile $header;
        print $texfile $table;
        print $texfile $footer;
    }
}

sub pdf {
    return unless $pdf;

    open my $incfile, "<", "../tex/inc.tex";
    my @oldinc = <$incfile>;
    open my $incfile, ">", "../tex/inc.tex";
    say $incfile '\includeonly{tex/random_testu01}';
    my $cmd;
    $cmd .= "cd ..;";
    $cmd .= " latexmk -f -silent";
    $cmd .= " -jobname=tab/random_testu01";
    $cmd .= " manual.tex";
    `$cmd`;
    open my $incfile, ">", "../tex/inc.tex";
    print $incfile $_ for @oldinc;
}

sub filter_txt {
    my $txt = "\Ltestu01/$subdir/random_testu01_$_[0].txt";
    return unless -e $txt;

    open my $txtfile, "<", $txt;
    my @lines = <$txtfile>;
    s/\s+$/\n/ for @lines;
    my $lines;
    $lines .= $_ for @lines;
    my @lines;
    while ($lines =~ s/(==+.*?tests were passed)//s) {
        if ($1 =~ /(.*All other tests were passed.*)/s) {
            push @lines, (split "\n", $1);
        }
    }
    if (@lines) {
        open my $txtfile, ">", $txt;
        for (@lines) {
            next if /^$/;
            next if /Version:/;
            next if /Total CPU time:/;
            next if /The following tests gave p-values outside/;
            next if /eps  means a value/;
            next if /eps1 means a value/;
            next if /Test\s+p-value/;
            say $txtfile $_;
            print $txtfile "\n" if /All other tests were passed/;
        }
    } else {
        unlink $txt;
    }
}

sub check_txt {
    my $txt = "\Ltestu01/$subdir/random_testu01_$_[0].txt";
    return unless -e $txt;

    shift;
    my ($b, $r, $u) = @_;

    open my $txtfile, "<", $txt;
    my @lines = <$txtfile>;
    my $lines = "@lines";
    if ($lines =~ /$u\n(.*?)All other tests were passed/s) {
        if ($1 =~ /--+\s*(.*?)\s*--+/s) {
            my @tests = split "\n", $1;
            my %this_failure;
            my %this_suspect;
            for (@tests) {
                my ($n, $p) = (split)[0, -1];
                if ($p < $failure or 1 - $p < $failure) {
                    $this_failure{$n} = 1;
                } elsif ($p < $suspect or 1 - $p < $suspect) {
                    $this_suspect{$n} = 1;
                }
            }
            $failure{$b}{$r}{$u}{$_} += 1 for keys %this_failure;
            $suspect{$b}{$r}{$u}{$_} += 1 for keys %this_suspect;
        }
    }
}
