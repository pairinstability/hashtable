hash table benchmarking
===

deps
---

the benchmarking use [google benchmark](https://github.com/google/benchmark), built as a dependency using the `src/cmake/GoogleBenchmark.cmake` in the project root directory. nothing needs to be done for this.

benchmarking also uses `boost::unordered_map`, so boost needs to be installed. this can be done in a variety of ways, but for testing this was done with ubuntu's package manager.

caveats
---

`simplehashmap` contains a very simple hashmap library to provide a baseline for the benchmarks. given this, the execution time may be shorter because of this simplicity at the sacrifice of not covering all edge cases in an optimized way which isn't measured in the benchmarks.