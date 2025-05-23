#include <benchmark/benchmark.h>

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

int add(int a, int b) { return a + b; }

namespace {

    void BM_Add(benchmark::State& state) {
        int a = 1, b = 2;
        for (auto _ : state) {
            benchmark::DoNotOptimize(add(a, b));
        }
    }
}  // namespace

BENCHMARK(BM_Add);

int main(int argc, char** argv) {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << "/home/marco/0_noteWork/2_application/0_tools/0_cppUtilis/2_cmake/"
           "01_cmke_template/logs/"
        << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S") << ".log";
    std::string log_file = oss.str();

    std::ofstream ofs(log_file);
    if (!ofs) {
        std::cerr << "Failed to create log file: " << log_file << std::endl;
        return 1;
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    ofs.close();

    return 0;
}
