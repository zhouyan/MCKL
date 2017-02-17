# ============================================================================
#  MCKL/example/math/math_asm.R
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
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
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

suppressPackageStartupMessages(library(ggplot2))

theme_set(theme_bw())
pdf("math_asm.pdf", width = 14.4, height = 9)

functions <- c("exp", "exp2", "expm1", "log", "log2", "log10", "log1p", "sin",
    "cos", "sincos", "tan")

for (f in functions) {
    dat <- read.table(paste0("math_", f, ".txt"), header = TRUE)
    dat$Range <- factor(dat$Range, ordered = TRUE, levels = unique(dat$Range))
    plt <- ggplot(dat)
    plt <- plt + aes(x = N, y = cpE)
    plt <- plt + aes(group = Library, color = Library)
    plt <- plt + geom_line()
    plt <- plt + geom_point()
    plt <- plt + scale_x_log10()
    plt <- plt + scale_y_log10()
    plt <- plt + facet_wrap(~Range, scale = "free_y")
    plt <- plt + ggtitle(f)
    print(plt)
}

gc <- dev.off()
