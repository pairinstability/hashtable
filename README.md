high performance (simple) hash table
===

WIP

* low memory footprint 
* fast (cache-friendly data structure)
* support for move-only and non-default constructor kv
* syntactically similar interface compared to `unordered_map`

more documentation is in `docs`.


dependencies
---

Tested on Ubuntu 22.04

benchmarking:

```sh
# install boost and libpfm
sudo apt-get install libboost-all-dev libpfm4-dev
# ensure libpfm can read perf events
sudo sh -c 'echo 0 > /proc/sys/kernel/perf_event_paranoid'
```

for more information about `perf_event_paranoid`, see ee [kernel docs on perf security](https://www.kernel.org/doc/html/latest/admin-guide/perf-security.html)

google benchmark is installed through cmake.

unit tests:

google test is installed through cmake.


usage
---

in short, `compile.sh` builds the benchmarking, examples, and unit tests and dependencies, and `run.sh` runs them.

build options:

```sh
./scripts/compile.sh                    # builds tests, examples, and benchmarking (everything)
./scripts/compile.sh examples           # builds the examples that use the hash table library
./scripts/compile.sh bench              # builds the benchmarking suite which uses the hash table library, a simple version, and other deps. uses googlebenchmark
./scripts/compile.sh tests              # builds the unit tests using the hash table library. uses googletest
```

run options:


```sh
./scripts/run.sh                        # runs the tests, examples, and bench in that order
./scripts/run.sh examples               # runs the examples
./scripts/run.sh bench                  # runs the benchmarking suite, with additional arguments passed to google benchmark
./scripts/run.sh benchgraph             # runs the benchmarking suite and produces graphs of the results, using python and matplotlib
./scripts/run.sh tests                  # runs the unit tests
```

any additional arguments supplied after `bench` will be passed as CLI arguments to google benchmark, see the [user guide](https://github.com/google/benchmark/blob/main/docs/user_guide.md). for example,

```sh
% ./scripts/run.sh bench --benchmark_filter="l "   
2023-12-30T22:44:24+00:00
Running /home/hubble/hashtable/build/runbench
Run on (8 X 2495.31 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 512 KiB (x8)
  L3 Unified 16384 KiB (x1)
Load Average: 0.03, 0.15, 0.16
------------------------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations cache-misses
------------------------------------------------------------------------------------------------------------
l LookupExistingElement SimpleHashTable/1031               1.55 ns         1.55 ns    385960697     18.0511u
l LookupExistingElement HashTable/1031                    0.344 ns        0.344 ns   2050634117     5.35542u
l LookupExistingElement std::unordered_map/1031            1.13 ns         1.13 ns    612335931     9.05875u
l LookupExistingElement boost::unordered_map/1031          3.50 ns         3.50 ns    200092250     41.5858u
l LookupNonexistentElement SimpleHashTable/1031            1.29 ns         1.29 ns    520155293     15.3858u
l LookupNonexistentElement HashTable/1031                 0.339 ns        0.339 ns   2089245369     3.57402u
l LookupNonexistentElement std::unordered_map/1031        0.978 ns        0.977 ns    720767050     13.5633u
l LookupNonexistentElement boost::unordered_map/1031      0.718 ns        0.718 ns   1028066041     23.1084u
l InsertRandomNumbers SimpleHashTable/1031                 6.97 ns         6.97 ns    100085527     240.354u
l InsertRandomNumbers HashTable/1031                      0.612 ns        0.612 ns   1094680975     2.55691u
l InsertRandomNumbers std::unordered_map/1031              24.0 ns         24.0 ns     29894863     2.61707m
l InsertRandomNumbers boost::unordered_map/1031            7.03 ns         7.03 ns    100477310     149.785u
l EraseElements SimpleHashTable/1031                       23.4 ns         23.4 ns     31042879     6.78313m
l EraseElements HashTable/1031                             12.9 ns         12.9 ns     55097251     264.913u
l EraseElements std::unordered_map/1031                    31.4 ns         31.4 ns     22511155      3.1519m
l EraseElements boost::unordered_map/1031                  24.2 ns         24.2 ns     29702860     4.89963m
```

only performs the benchmarks whose name matches that regex pattern, in this case for benchmarks on the large number of buckets (name prepended by l-space)




benchmarking
---
 
this hash table has been benchmarked against a simple hash table, std::unordered_map, and boost::unordered_map with google benchmark. python scripts use matplotlib to generate plots of this data.

the `bench/` directory contains the tools necessary for this. See `docs/benchmarking.md`
