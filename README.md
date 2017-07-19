# Cholera Deaths

Examples of three MCMC sampling algorithms for estimating the posterior of a
univariate Poisson-Gamma model, using the 1854 Broad Street Cholera Outbreak
dataset. Samplers include a generic Metropolis algorithm with parallelized
log-likelihood calculations, diminishing adaption and naive parallel
prefetching. OpenMP is used for parallel computing.


## Use

Ensure that a version of clang with OpenMP support is available. If issues
linking against OpenMP are encountered, check that clang is able to find the
relevant headers (`libomp-dev` may need to be installed on Debian based
systems).

Setup the project directory and build the sampling program.

```
mkdir build bin
make data/cholera_counts.txt
make bin/sampler
```

Run the sampler using e.g. prefetching.

```
./bin/sampler prefetching data/cholera_counts.txt > samples.txt
```
