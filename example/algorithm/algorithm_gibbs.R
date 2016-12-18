# ============================================================================
#  MCKL/example/algorithm/algorithm_gibbs.R
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

library(ggplot2)

obs <- read.table("algorithm_gibbs.data", header = FALSE)[,1]
est <- read.table("algorithm_gibbs.save", header = TRUE)
dat <- data.frame(mu = est$V0.0, lambda = est$V0.1)
plt <- qplot(x = mu, y = lambda, data = dat, geom = "density_2d")
plt <- plt + geom_vline(xintercept = mean(obs), color = "green")
plt <- plt + geom_hline(yintercept = 1 / var(obs), color = "green")
plt <- plt + geom_vline(xintercept = mean(dat$mu), color = "red")
plt <- plt + geom_hline(yintercept = mean(dat$lambda), color = "red")
plt <- plt + theme_bw()

pdf("algorithm_gibbs.pdf", width = 5, height = 5)
print(plt)
gc <- dev.off()
