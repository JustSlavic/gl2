#include <benchmark/benchmark.h>
#include <vector>

static void bench_push_back(benchmark::State& state) {
    while (state.KeepRunning()) {
        std::vector<int> v;
        v.push_back(42);
    }
}
BENCHMARK(bench_push_back);

BENCHMARK_MAIN();
