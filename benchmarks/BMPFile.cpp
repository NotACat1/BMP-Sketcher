#include <benchmark/benchmark.h>
#include "BMPFile.hpp"
#include <fstream>
#include <filesystem>

using namespace std;

static void BM_Create(benchmark::State& state) {
    for (auto _ : state) {
        BMPFile bmp;
        bmp.create(state.range(0), state.range(1), BMPFile::PixelFormat::BGRA32, {255, 0, 0});
        benchmark::DoNotOptimize(bmp);
    }
}
BENCHMARK(BM_Create)->Args({512, 512})->Args({1024, 1024});

static void BM_SetPixel(benchmark::State& state) {
    BMPFile bmp;
    bmp.create(512, 512, BMPFile::PixelFormat::BGRA32, {});
    BMPFile::Pixel px(123, 231, 77);
    for (auto _ : state) {
        bmp.setPixel(state.range(0), state.range(1), px);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_SetPixel)->Args({100, 100});

static void BM_GetPixel(benchmark::State& state) {
    BMPFile bmp;
    bmp.create(512, 512, BMPFile::PixelFormat::BGRA32, {});
    for (auto _ : state) {
        auto px = bmp.getPixel(state.range(0), state.range(1));
        benchmark::DoNotOptimize(px);
    }
}
BENCHMARK(BM_GetPixel)->Args({100, 100});

static void BM_FlipVertically(benchmark::State& state) {
    BMPFile bmp;
    bmp.create(state.range(0), state.range(1), BMPFile::PixelFormat::BGRA32, {});
    for (auto _ : state) {
        bmp.flipVertically();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_FlipVertically)->Args({512, 512})->Args({1024, 1024});

static void BM_ConvertToBlackAndWhite(benchmark::State& state) {
    BMPFile bmp;
    bmp.create(state.range(0), state.range(1), BMPFile::PixelFormat::BGRA32, {123, 231, 77});
    for (auto _ : state) {
        bmp.convertToBlackAndWhite();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_ConvertToBlackAndWhite)->Args({512, 512})->Args({1024, 1024});

static void BM_Save(benchmark::State& state) {
    BMPFile bmp;
    bmp.create(state.range(0), state.range(1), BMPFile::PixelFormat::BGRA32, {123, 231, 77});
    string filename = std::filesystem::temp_directory_path() / "bench_save.bmp";

    for (auto _ : state) {
        bmp.save(filename);
    }

    std::filesystem::remove(filename);
}
BENCHMARK(BM_Save)->Args({512, 512})->Args({1024, 1024});

static void BM_Load(benchmark::State& state) {
    BMPFile bmp;
    bmp.create(state.range(0), state.range(1), BMPFile::PixelFormat::BGRA32, {255, 0, 0});
    string filename = std::filesystem::temp_directory_path() / "bench_load.bmp";
    bmp.save(filename);

    for (auto _ : state) {
        BMPFile bmp2;
        bmp2.load(filename);
        benchmark::DoNotOptimize(bmp2);
    }

    std::filesystem::remove(filename);
}
BENCHMARK(BM_Load)->Args({512, 512})->Args({1024, 1024});

BENCHMARK_MAIN();
