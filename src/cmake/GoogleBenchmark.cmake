# google benchmark cmake for building the dependency

# dont care about unit testing google benchmark
set(BENCHMARK_ENABLE_TESTING NO)
# for performance counters
set(BENCHMARK_ENABLE_LIBPFM ON)

include(FetchContent)

find_program(CLANGXX_COMPILER NAMES clang++)
find_program(CLANG_COMPILER NAMES clang)

if(CLANGXX_COMPILER AND CLANG_COMPILER)
    set(CMAKE_CXX_COMPILER "${CLANGXX_COMPILER}")
    set(CMAKE_C_COMPILER "${CLANG_COMPILER}")
endif()

find_program(MOLD_LINKER NAMES mold)
if(MOLD_LINKER)
    set(CMAKE_LINKER "${MOLD_LINKER}")
endif()

FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG origin/main
)

FetchContent_MakeAvailable(googlebenchmark)