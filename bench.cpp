#include <benchmark/benchmark.h>

#include "american_english.hpp"
#include "metropolitan_french.hpp"
#include "phonology.hpp"

static void BM_french(benchmark::State& state) {
  phonology::MetropolitanFrench mf;
  for (auto _ : state) {
    benchmark::DoNotOptimize(phonology::get_word(mf, 1));
  }
}
BENCHMARK(BM_french);

static void BM_english(benchmark::State& state) {
  phonology::AmericanEnglish ae;
  for (auto _ : state) {
    benchmark::DoNotOptimize(phonology::get_word(ae, 1));
  }
}
BENCHMARK(BM_english);

BENCHMARK_MAIN();
