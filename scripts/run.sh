#!/bin/bash

set -e

OS_NAME=$(uname -s)
ROOT_SRC_DIR="$(cd -P "$(dirname "${BASH_SOURCE[0]}")/.." && pwd )"

if [ ! -d "build" ]; then
    echo "no build directory. is a target built?"
    exit 1
fi

if [ $# -gt 0 ]; then
    RUN_ALL=false
    for arg in "$@"; do
        case $arg in
            "bench")
                RUN_bench=true
                ;;
            "benchgraph")
                RUN_benchgraph=true
                ;;
            "tests" | "examples")
                eval "RUN_$arg=true"
                ;;
            *)
                if [ "$RUN_bench" = true ]; then
                    BENCHMARK_ARGS+=("$arg")
                else
                    echo "Unknown argument"
                    exit 1
                fi
                ;;
        esac
    done
fi

if [ "$RUN_ALL" = true ]; then
    for i in tests, examples
    do
        "${ROOT_SRC_DIR}/build/run${i}"
    done
else
    for arg in "$@"; do
        if [ "$(eval echo \$RUN_$arg)" = true ]; then
            if [ "$arg" = "bench" ]; then
                if [ "$OS_NAME" = "Linux" ]; then
                    taskset -c 0 "${ROOT_SRC_DIR}/build/runbench" --benchmark_perf_counters=cache-misses --benchmark_counters_tabular=true "${BENCHMARK_ARGS[@]}"
                fi
            elif [ "$arg" = "benchgraph" ]; then
                echo "2"
                # run the bench and plot them
                # https://github.com/google/benchmark/blob/main/docs/user_guide.md
            else
                "${ROOT_SRC_DIR}/build/run${arg}"
            fi
        fi
    done
fi
