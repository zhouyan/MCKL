#!/usr/bin/env perl

# ============================================================================
#  MCKL/manual/tab/math_vml.pl
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

`mkdir -p math_vml`;

my $run = 0;
my $build = 0;
my $llvm = "../../build/release-llvm";
my $gnu = "../../build/release-gnu";
my $intel = "../../build/release-intel";
my $compiler = "llvm";
my $name;
my $write = 0;
my $pdf = 0;
my $scale = 1;
GetOptions(
    "run"        => \$run,
    "build"      => \$build,
    "llvm=s"     => \$llvm,
    "gnu=s"      => \$gnu,
    "intel=s"    => \$intel,
    "compiler=s" => \$compiler,
    "name=s"     => \$name,
    "write"      => \$write,
    "pdf"        => \$pdf,
    "scale"      => \$scale,
);
$build = 1 if $run;

my @keys = qw(cos exp exp2 expm1 log log10 log1p log2 sin sincos sqrt tan);

my @function;
for my $k (@keys) {
    push @function, $k if $k eq $name or !$name;
}

my $cpuid = `cpuid_info`;
my $simd;
$simd = "sse2" if $cpuid =~ "SSE2";
$simd = "avx2" if $cpuid =~ "AVX2";
my @simd = qw(sse2 avx2);
unless ($scale) {
    $scale = 1;
    $scale *= 3.33 / 3.06 if $simd eq "sse2";
    $scale *= 3.80 / 2.90 if $simd eq "avx2";
}

my %compiler = (llvm => $llvm, gnu => $gnu, intel => $intel);
my @compiler = qw(llvm gnu intel);

my %cpe_s;
my %cpe_v;
my %cpe_a;

&build;
&run;
# &read;
# &table;
# &pdf;

sub build {
    return unless $build;

    my @target;
    push @target, "\Lmath_${_}" for @function;
    if (@target) {
        for my $c (@compiler) {
            my $d = $compiler{$c};
            next if (not -d $d);
            say $d;
            if ($name) {
                `ninja -C $d @target 2>&1`;
            } else {
                `ninja -C $d math 2>&1`;
            }
        }
    }
}

sub run {
    return unless $run;

    for my $c (@compiler) {
        my $result;
        my $d = $compiler{$c};
        next if (not -d $d);
        say $d;
        for my $r (@function) {
            my @result;
            my $cmd = "ninja -C $d math_${r}-check 2>&1";
            my @lines = `$cmd`;
            @lines = grep { /^vd/ } @lines;
            push @result, @lines;
            print $_ for @lines;
            if ($write) {
                open my $txt, ">", "math_vml/" .
                "\Lmath_${r}_${c}_${simd}.txt";
                print $txt $_ for @result;
            }
        }
    }

}

# sub read {
#     for my $s (@simd) {
#         for my $r (@distribution) {
#             open my $txt, "<", "random_distribution/" .
#             "\Lrandom_distribution_${r}_${compiler}_${s}.txt";
#             my @lines = <$txt>;
#             my @result = grep { /$r<(double|int64_t)>/ } @lines;
#             for (@result) {
#                 my ($name, $cpe_s, $cpe_m, $cpe_v, $cpe_i, $lib) = (split);
#                 $cpe_s *= $scale;
#                 $cpe_m *= $scale;
#                 $cpe_v *= $scale;
#                 $cpe_i *= $scale;
#                 $name =~ s/(.*)<.*>(.*)/$1$2/;
#                 $cpe_s{$s}{$name} = 0xFFFF unless $cpe_s{$s}{$name};
#                 $cpe_m{$s}{$name} = 0xFFFF unless $cpe_m{$s}{$name};
#                 $cpe_f{$s}{$name} = 0xFFFF unless $cpe_f{$s}{$name};
#                 $cpe_l{$s}{$name} = 0xFFFF unless $cpe_l{$s}{$name};
#                 $cpe_i{$s}{$name} = 0xFFFF unless $cpe_i{$s}{$name};
#                 if ($lib eq "VMF") {
#                     $cpe_s{$s}{$name} = $cpe_s if $cpe_s < $cpe_s{$s}{$name};
#                     $cpe_m{$s}{$name} = $cpe_m if $cpe_m < $cpe_m{$s}{$name};
#                     $cpe_f{$s}{$name} = $cpe_v if $cpe_v < $cpe_f{$s}{$name};
#                     $cpe_i{$s}{$name} = $cpe_i if $cpe_i < $cpe_i{$s}{$name};
#                 }
#                 if ($lib eq "VML") {
#                     $cpe_s{$s}{$name} = $cpe_s if $cpe_s < $cpe_s{$s}{$name};
#                     $cpe_m{$s}{$name} = $cpe_m if $cpe_m < $cpe_m{$s}{$name};
#                     $cpe_l{$s}{$name} = $cpe_v if $cpe_v < $cpe_l{$s}{$name};
#                     $cpe_i{$s}{$name} = $cpe_i if $cpe_i < $cpe_i{$s}{$name};
#                 }
#             }
#         }
#     }
# }
# 
# sub table {
#     my $header;
#     $header .= '\tnumshape' . "\n";
#     $header .= '\begin{tabularx}{\textwidth}{p{2in}RRRRR}' . "\n";
#     $header .= ' ' x 2 . '\toprule' . "\n";
#     $header .= ' ' x 2 . 'Distribution & \std & \mckl & \vmf & \vml & \mkl';
#     $header .= " \\\\\n";
#     $header .= ' ' x 2 . '\midrule' . "\n";
# 
#     my $footer;
#     $footer .= ' ' x 2 . '\bottomrule' . "\n";
#     $footer .= '\end{tabularx}' . "\n";
# 
#     for my $k (@keys) {
#         for my $s (@simd) {
#             my $table;
#             for my $r (@{$distribution{$k}}) {
#                 my @name = sort grep { /^$r(\(|$)/ } keys %{$cpe_s{$s}};
#                 for my $name (@name) {
#                     $table .= " " x 2 . sprintf("%-40s", "\\texttt{$name}");
#                     $table .= " & ";
#                     if ($nostd{$r}) {
#                         $table .= &format("--");
#                     } else {
#                         $table .= &format($cpe_s{$s}{$name});
#                     }
#                     $table .= " & " . &format($cpe_m{$s}{$name});
#                     $table .= " & " . &format($cpe_f{$s}{$name});
#                     $table .= " & " . &format($cpe_l{$s}{$name});
#                     $table .= " & ";
#                     if ($nomkl{$r}) {
#                         $table .= &format("--");
#                     } else {
#                         $table .= &format($cpe_i{$s}{$name});
#                     }
#                     $table .= "\\\\\n";
#                 }
#             }
#             if ($table) {
#                 open my $texfile, ">",
#                 "\Lrandom_distribution_${k}_${s}.tex";
#                 print $texfile $header;
#                 print $texfile $table;
#                 print $texfile $footer;
#             }
#         }
#     }
# }
# 
# sub pdf {
#     return unless $pdf;
# 
#     open my $incfile, "<", "../tex/inc.tex";
#     my @oldinc = <$incfile>;
#     open my $incfile, ">", "../tex/inc.tex";
#     say $incfile '\includeonly{tex/random_distribution}';
#     my $cmd;
#     $cmd .= "cd ..;";
#     $cmd .= " latexmk -f -silent";
#     $cmd .= " -jobname=tab/random_distribution";
#     $cmd .= " manual.tex";
#     `$cmd`;
#     open my $incfile, ">", "../tex/inc.tex";
#     print $incfile $_ for @oldinc;
# }
# 
# sub distribution_name {
#     my $name = shift;
#     if ($name eq "U01Canonical") {
#         $name = "U01_Canonical";
#     } elsif ($name =~ /U01(..)/) {
#         $name =~ s/U01(..)/U01_$1/;
#     } else {
#         $name =~ s/([A-Z])/_$1/g;
#         $name =~ s/^_//g;
#     }
#     $name
# }
# 
# sub format
# {
#     my $num = shift;
#     if ($num eq "--") {
#         sprintf("%-6s", $num);
#     } elsif ($num > 100) {
#         sprintf('%-6.0f', $num);
#     } elsif ($num > 10) {
#         sprintf('%-6.1f', $num);
#     } else {
#         sprintf('%-6.2f', $num);
#     }
# }