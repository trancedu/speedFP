#include <chrono>
#include <vector>
#include <memory>
#include <iostream>

constexpr size_t ITERATIONS = 10'000;
constexpr size_t SAMPLE_SIZE = ITERATIONS;  // 2 data points per iteration 

template <typename Func>
void benchmark(const std::string& label, Func func, size_t iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < iterations; ++i) {
        func();
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double average_ns = static_cast<double>(total_ns) / iterations / SAMPLE_SIZE;
    std::cout << label << " - Average: " << average_ns << " ns/iter\n";
}
