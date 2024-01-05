# building hash table benchmarking

# google benchmark
include(../cmake/GoogleBenchmark.cmake)

# boost
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_executable(runbench bench.cpp)
target_link_libraries(runbench PRIVATE libsimplehashtable libhashtable benchmark::benchmark ${Boost_LIBRARIES} absl::hash)