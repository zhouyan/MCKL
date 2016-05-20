# ============================================================================
#  MCKL/manual/prg/pf.R
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

library(ggplot2)
library(tikzDevice)

pf <- read.table("pf.out", header = TRUE)
sink("pf.rout")
print(pf[1:5,])
sink()

obs <- read.table("pf.data", header = FALSE)
dat <- data.frame(
    X = c(pf[["pos.0"]], obs[,1]),
    Y = c(pf[["pos.1"]], obs[,2]))
dat[["Source"]] <- rep(c("Estimate", "Observation"), each = dim(obs)[1])
plt <- qplot(x = X, y = Y, data = dat, geom = "path")
plt <- plt + aes(group = Source, color = Source, linetype = Source)
plt <- plt + xlab("$X$")
plt <- plt + ylab("$Y$")
plt <- plt + theme_bw() + theme(legend.position = "top")

tikz("pf.tex", width = 4.5, height = 4.5, standAlone = TRUE)
print(plt)
dev.off()
system("latexmk -silent -f pf.tex &>/dev/null")
system("latexmk -c pf.tex &>/dev/null")
system("rm -f pf.tex")
