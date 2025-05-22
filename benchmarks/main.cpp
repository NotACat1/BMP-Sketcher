#include <benchmark/benchmark.h>
#include "example.hpp"

static void BM_Hello(benchmark::State& state) {
    for (auto _ : state) {
        auto str = bmp::hello();
        benchmark::DoNotOptimize(str);
    }
}
BENCHMARK(BM_Hello);

BENCHMARK_MAIN();