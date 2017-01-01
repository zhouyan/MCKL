# ============================================================================
#  MCKL/example/algorithm/algorithm_pf.R
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

backend <- c("SEQ", "STD", "OMP", "TBB")

obs <- read.table("algorithm_pf.data", header = FALSE)
obs <- data.frame(X = obs[,1], Y = obs[,2])
obs$Source <- "Observation"

dat <- data.frame()
for (bkd in backend) {
    savefile <- paste0("algorithm_pf_", bkd, ".save")
    if (file.exists(savefile)) {
        d <- read.table(savefile, header = FALSE)
        sel <- data.frame(X = d[,1], Y = d[,2])
        res <- data.frame(X = d[,3], Y = d[,4])
        mul <- data.frame(X = d[,5], Y = d[,6])
        sel$Source <- "Selection"
        res$Source <- "Resample"
        mul$Source <- "Mutation"
        d <- rbind(sel, res, mul, obs)
        d$Backend <- bkd
        dat <- rbind(dat, d)
    }
}

plt <- qplot(x = X, y = Y, data = dat, geom = "path")
plt <- plt + aes(group = Source, color = Source, linetype = Source)
plt <- plt + facet_wrap(~ Backend)
plt <- plt + theme_bw()
plt <- plt + theme(legend.position = "top")
plt <- plt + theme(legend.title = element_blank())
pdf("algorithm_pf.pdf")
print(plt)
gc <- dev.off()
