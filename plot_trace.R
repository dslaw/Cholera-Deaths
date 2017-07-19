require(ggplot2)
require(reshape2)


samplers <- c("adaptive", "parallel", "prefetching")
chains <- lapply(samplers, function(sampler) {
                    cmd <- paste("./bin/sampler", sampler, "data/cholera_counts.txt")
                    as.numeric(system(cmd, intern = TRUE))
                 })

max_len <- max(sapply(chains, length))
draws <- data.frame(adaptive    = chains[[1]][1:max_len],
                    parallel    = chains[[2]][1:max_len],
                    prefetching = chains[[3]][1:max_len],
                    iter        = seq_len(max_len))

draws_long <- melt(draws, id.vars = "iter",
                   variable.name = "sampler",
                   value.name = "draw")

p <- ggplot(draws_long, aes(iter, draw, color = sampler)) +
    geom_line()

ggsave("figs/trace.png", p)
