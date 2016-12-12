#!/usr/bin/env perl

# ============================================================================
#  MCKL/tidy.pl
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

my $clang_format = "/opt/LLVM/bin/clang-format";
GetOptions("clang_format=s" => \$clang_format);

my $root = $0;
$root =~ s/\/tidy.pl//;
my $nroot = length($root);

&update($root);

sub update
{
    my $dir = shift;
    my @ls = `ls $dir`;
    for (@ls) {
        chomp;
        my $path = $dir . "/" . $_;
        if (-f $path) {
            next if $path =~ /NEWS\.md$/;
            next if $path =~ /README\.md$/;
            next if $path =~ /\.aux$/;
            next if $path =~ /\.bbl$/;
            next if $path =~ /\.bcf$/;
            next if $path =~ /\.blg$/;
            next if $path =~ /\.fdb_latexmk$/;
            next if $path =~ /\.fls$/;
            next if $path =~ /\.latexmain$/;
            next if $path =~ /\.log$/;
            next if $path =~ /\.out$/;
            next if $path =~ /\.pdf$/;
            next if $path =~ /\.run.xml$/;
            next if $path =~ /\.toc$/;
            next if $path =~ /config\/.*\.md\.in$/;
            next if $path =~ /example\/gmm\/gmm\.data$/;
            next if $path =~ /example\/pf\/pf\.data$/;
            next if $path =~ /example\/pf\/pf\.truth$/;
            next if $path =~ /example\/random\/random_.*\.txt$/;
            next if $path =~ /tidy\.pl$/;
            next if $path =~ /inc\.tex$/;
            next if $path =~ /manual\/tab\/random_.*\.tex$/;
            &filename($path);
            &copyright($path);
            if ($path =~ /\.(c|h|cpp|hpp|cl|cpp\.in)$/) {
                system "$clang_format -i $path";
            }
        } elsif (-d $path) {
            next if $path =~ ".git";
            next if $path =~ "build";
            next if $path =~ "manual/tab/random_distribution";
            next if $path =~ "manual/tab/random_rng";
            next if $path =~ "manual/tab/random_testu01";
            &update($path);
        }
    }
}

sub filename
{
    my $file = shift;
    open my $in, "<", $file;
    my @lines = <$in>;

    substr($file, 0, $nroot) = "MCKL";

    if ($file =~ /\.hpp$/ or $file =~ /\.h$/) {
        my $guard = $file;
        $guard =~ s/\//_/g;
        $guard =~ s/\./_/g;
        $guard =~ s/^MCKL_include_//;
        $guard =~ s/^MCKL_example_.*_include/MCKL_EXAMPLE/;
        $guard = "\U$guard";
        my @found = grep { /$guard/ } @lines;
        say "Incorrect header guard: $file: $guard" unless @found == 3;
    }

    open my $out, ">", $file;

    my $line = shift @lines;
    print $out $line;

    if ($line =~ /^#!/) {
        my $line = shift @lines;
        print $out $line;
        my $line = shift @lines;
        print $out $line;
    }

    $line = shift @lines;
    if ($line =~ /^(.. )MCKL\//) {
        print $out $1, $file, "\n";
    } else {
        print $out $line;
    }

    print $out @lines;
}

sub copyright
{
    my $file = shift;
    open my $in, '<', $file;
    my @lines = <$in>;

    my $found = 0;
    for (@lines) {
        if (/Copyright/) {
            $found = 1;
            $_ =~ s/2013-2016/2013-2017/;
            last;
        }
    }

    open my $out, '>', $file;
    print $out @lines;
    say "No copyright information: $file" unless $found;
}
