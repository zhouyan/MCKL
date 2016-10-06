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

my $failure = 1e-10;
my $suspect = 1e-4;
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

my %instd;
$instd{$_} = 1 for (@std);

my %rngs = (
    std      => [@std],
    aesni    => [@aesni],
    philox   => [@philox],
    threefry => [@threefry],
    mkl      => [@mkl],
    rdrand   => [@rdrand],
);

my @keys = qw(std aesni philox threefry mkl);

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

sub table
{
    my $b = shift;

    my $header;
    $header .= '\begin{tabularx}{\textwidth}{p{1.5in}RRRRRR}' . "\n";
    $header .= '\toprule' . "\n";
    $header .= '\rng';
    $header .= ' & \std';
    $header .= ' & \textsc{u01}';
    $header .= ' & \textsc{u01cc}';
    $header .= ' & \textsc{u01co}';
    $header .= ' & \textsc{u01oc}';
    $header .= ' & \textsc{u01oo}';
    $header .= " \\\\\n";
    $header .= '\midrule' . "\n";

    my $footer;
    $footer .= '\bottomrule' . "\n";
    $footer .= '\end{tabularx}' . "\n";

    my $table;
    for my $k (@keys) {
        my @val = @{$rngs{$k}};
        for my $r (@val) {
            my $txt = "random_testu01_\L${b}_$r.txt";
            if (-e $txt) {
                $table .= &filter($txt, $b, $r);
            }
        }
    }
    $table = $header . $table . $footer if $table;
    $table;
}

sub filter
{
    my $txt = shift;
    my $bat = shift;
    my $rng = shift;
    open my $txtfile, '<', $txt;
    my @lines = <$txtfile>;
    s/\s+$/\n/ for @lines;
    my $lines = "@lines";

    $lines =~ /(STD\n.*?tests were passed)/s;   my $STD   = $1;
    $lines =~ /(U01\n.*?tests were passed)/s;   my $U01   = $1;
    $lines =~ /(U01CC\n.*?tests were passed)/s; my $U01CC = $1;
    $lines =~ /(U01CO\n.*?tests were passed)/s; my $U01CO = $1;
    $lines =~ /(U01OC\n.*?tests were passed)/s; my $U01OC = $1;
    $lines =~ /(U01OO\n.*?tests were passed)/s; my $U01OO = $1;

    ($STD,   my $detailSTD)   = &check($STD);
    ($U01,   my $detailU01)   = &check($U01);
    ($U01CC, my $detailU01CC) = &check($U01CC);
    ($U01CO, my $detailU01CO) = &check($U01CO);
    ($U01OC, my $detailU01OC) = &check($U01OC);
    ($U01OO, my $detailU01OO) = &check($U01OO);


    my $tline;
    if ($STD or $U01 or $U01CC or $U01CO or $U01OC or $U01OO) {
        if ($verbose) {
            say '=' x 80;
            say "$bat $rng";
            say '-' x 80;
            &detail("STD",   $detailSTD);
            &detail("U01",   $detailU01);
            &detail("U01CC", $detailU01CC);
            &detail("U01CO", $detailU01CO);
            &detail("U01OC", $detailU01OC);
            &detail("U01OO", $detailU01OO);
            say '-' x 80;
        }
        $STD   = '--' if not $STD;
        $U01   = '--' if not $U01;
        $U01CC = '--' if not $U01CC;
        $U01CO = '--' if not $U01CO;
        $U01OC = '--' if not $U01OC;
        $U01OO = '--' if not $U01OO;
        $rng =~ s/_/\\_/g;
        $tline .= "\\texttt{$rng}";
        $tline .= " & $STD";
        $tline .= " & $U01";
        $tline .= " & $U01CC";
        $tline .= " & $U01CO";
        $tline .= " & $U01OC";
        $tline .= " & $U01OO";
        $tline .= " \\\\\n";
    }
    $tline;
}

sub check
{
    my $lines = shift;
    my $tests;
    $tests = $1 if ($lines =~ /--+\s*(.*?)\s*--+/s);
    my @tests = split "\n", $tests;

    my $nfailure = 0;
    my $nsuspect = 0;
    my $details;
    for (@tests) {
        s/ *(.*) */$1/;
        $details .= $_ . "\n";
        my $pval = (split)[-1];
        $pval = 1 - $pval;
        if ($pval < $failure or 1 - $pval < $failure) {
            $nfailure++;
        } elsif ($pval < $suspect or 1 - $pval < $suspect) {
            $nsuspect++;
        }
    }

    my $result;
    $result .= $nfailure if $nfailure;
    $result .= " \\textcolor{MGrey}{($nsuspect)}" if $nsuspect;

    ($result, $details);
}

sub detail
{
    my $u01 = shift;
    my @detail = split "\n", $_[0];
    if (@detail) {
        printf('%-6s', $u01);
        say &format($detail[0]);
        shift @detail;
        say ' ' x 6, &format($_) for @detail;
    }
}

sub format
{
    my $line = shift;
    my $num = (split ' ', $line)[0];
    if ($num < 10) {
        $line = '  ' . $line;
    } elsif ($num < 100) {
        $line = ' ' . $line;
    }
    $line;
}
