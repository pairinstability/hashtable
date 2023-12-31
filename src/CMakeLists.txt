add_subdirectory(lib)

option(BUILD_BENCH "Build benchmarking" ON)
if (BUILD_BENCH)
    add_subdirectory(bench/simplehashmap/lib)
    add_subdirectory(bench)
endif()

option(BUILD_TESTS "Build unit tests" ON)
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()

option(BUILD_EXAMPLES "Build examples" ON)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()