# ============================================================================
#  MCKL/example/pf/pf_cv.R
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
suppressPackageStartupMessages(library(rhdf5))

##############################################################################

theme_set(theme_bw())
pdf("pf_cv.pdf", width = 14.4, height = 9)

##############################################################################

smp <- paste0("Backend", c("SEQ", "STD", "OMP", "TBB"))
exe <- paste("pf_cv", smp, sep = ".")
res <- c("Multinomial", "Stratified", "Systematic", "Residual",
    "ResidualStratified", "ResidualSystematic")
rc <- c("RowMajor", "ColMajor")
rs <- c("RNGSetVector", "RNGSetTBB", "RNGSetTBBKPI")
runs <- expand.grid(exe, res, rc, rs)
runs <- paste(runs$Var1, runs$Var2, runs$Var3, runs$Var4, sep = ".")

##############################################################################

dat <- data.frame()
for (run in runs) {
    pf_cv.err.txt <- paste0(run, ".err.txt")
    if (file.exists(pf_cv.err.txt)) {
        dat <- rbind(dat, read.table(pf_cv.err.txt, header = TRUE))
    }
}
plt <- ggplot(dat)
plt <- plt + aes(x = MatrixLayout, y = Error)
plt <- plt + aes(group = RNGSetType, fill = RNGSetType)
plt <- plt + geom_bar(stat = "identity", position = "dodge")
plt <- plt + facet_grid(ResampleScheme ~ Backend, scale = "free_y")
print(plt)

truth <- read.table("pf_cv.truth", header = FALSE)
n <- dim(truth)[1]

lvl <- c("Estimates", "Truth")
grp <- rep(lvl, each = n)
src <- factor(grp, ordered = TRUE, levels = lvl)
est <- rep("Estimates", n)

err.list <- list()
dat.list <- list()
dat <- data.frame(
    Position.X = truth[,1],
    Position.Y = truth[,2],
    Group = rep("Truth", n),
    Source = rep("Truth", n))

pf_cv <- function(filename, sampler)
{
    err.x <- sampler$pos.x - truth[,1]
    err.y <- sampler$pos.y - truth[,2]
    error <- mean(sqrt(err.x * err.x + err.y * err.y))
    .GlobalEnv$err.list[[filename]] <- error

    .GlobalEnv$dat.list[[filename]] <- data.frame(
        Position.X = c(sampler$pos.x, truth[,1]),
        Position.Y = c(sampler$pos.y, truth[,2]),
        Group = grp,
        Source = src)

    .GlobalEnv$dat <- rbind(.GlobalEnv$dat, data.frame(
            Position.X = sampler$pos.x,
            Position.Y = sampler$pos.y,
            Group = rep(filename, n),
            Source = est))
}

for (run in runs) {
    pf_cv.txt <- paste0(run, ".txt")
    if (file.exists(pf_cv.txt)) {
        pf_cv(pf_cv.txt, read.table(pf_cv.txt, header = TRUE))
    }

    pf_cv.h5 <- paste0(run, ".h5")
    if (file.exists(pf_cv.h5)) {
        pf_cv.s <- paste(pf_cv.h5, "(Sampler)")
        pf_cv(pf_cv.s, as.data.frame(h5read(pf_cv.h5, "Sampler")))

        pf_cv.m <- paste(pf_cv.h5, "(Monitor)")
        pf_cv(pf_cv.m, as.data.frame(h5read(pf_cv.h5, "Monitor")))

        pf_cv.p <- paste(pf_cv.h5, "(Particle)")
        particle <- h5read(pf_cv.h5, "Particle")
        pos.x <- numeric()
        pos.y <- numeric()
        for (i in 0:(length(particle) - 1)) {
            name <- paste0("Iter.", i)
            s <- particle[[name]]$State
            w <- particle[[name]]$Weight
            if (dim(s)[1] > dim(s)[2]) {
                pos.x <- c(pos.x, sum(w * s[,1]))
                pos.y <- c(pos.y, sum(w * s[,2]))
            } else {
                pos.x <- c(pos.x, sum(w * s[1,]))
                pos.y <- c(pos.y, sum(w * s[2,]))
            }
        }
        pf_cv(pf_cv.p, as.data.frame(cbind(pos.x, pos.y)))
    }
}
dat$Source <- factor(dat$Source, ordered = TRUE, levels = lvl)

plt <- ggplot(dat)
plt <- plt + aes(x = Position.X, y = Position.Y)
plt <- plt + aes(group = Group, color = Source, linetype = Source)
plt <- plt + geom_path()
print(plt)

for (p in names(err.list)) {
    e <- err.list[[p]]
    t <- paste(p, " (Error = ", e, ")")
    print(plt %+% dat.list[[p]] + ggtitle(t))
}

##############################################################################

garbage <- dev.off()
