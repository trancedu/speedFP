# Financial Design Patterns Performance Benchmark

This project measures the performance characteristics of various C++ design patterns used in financial data processing systems. The focus is on the interaction between data classes and pricer classes, testing the speed of different design patterns such as virtual functions, CRTP, dynamic cast, static cast, fat interface, and derived functions.

## Design Patterns Tested
- Virtual function dispatch
- Curiously Recurring Template Pattern (CRTP)
- Dynamic cast type checking
- Static cast with type flag
- Fat interface with virtual members
- Derived pricer pattern
- Variant-based dispatch

## Requirements
- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- CMake 3.12+
- 64-bit system recommended

## Class Overview

### Data Class
The `Data` class serves as a base class holding common data for both `StockData` and `OptionData`. It provides a common interface and shared data members that are used by its derived classes.

### Pricer Class
The `Pricer` class is designed to take a pointer to a `Data` class object, which could be either a base or derived class. The `OptionPricer` and `StockPricer` classes inherit from `Pricer` and implement specific pricing logic for options and stocks, respectively.

## How to Run

### Windows (MSVC)
1. Open a terminal and navigate to the project directory.
2. Create a build directory and navigate into it:
   ```shell
   mkdir build
   cd build
   ```
3. Run CMake to configure the project:
   ```shell
   cmake -B build -G "Visual Studio 17 2022" -A x64
   ```
4. Build the project:
   ```shell
   cmake --build build --config Release
   ```
5. Run the benchmarks using the PowerShell script:
   ```shell
   .\run_benchmarks.ps1
   ```

### Linux/MacOS
1. Open a terminal and navigate to the project directory.
2. Create a build directory and navigate into it:
   ```shell
   mkdir build
   cd build
   ```
3. Run CMake to configure the project:
   ```shell
   cmake ..
   ```
4. Build the project:
   ```shell
   make -j$(nproc)
   ```
5. Make the benchmark script executable and run it:
   ```shell
   chmod +x run_benchmarks.sh
   ./run_benchmarks.sh
   ```

## Performance Statistics

All results were obtained on a MacBook Pro with an M1 Pro processor and 16GB of RAM, running clang++ or g++ installed by homebrew or running a Windows 11 virtual machine with MSVC 2022.

### Optimization Levels
- **O1**: This optimization level focuses on reducing code size and improving execution speed without significantly increasing compilation time.
- **O2**: This level provides more aggressive optimizations, aiming to improve execution speed further, often at the cost of increased compilation time.
- **MSVC Release Mode**: Similar to O2, this mode optimizes for speed and efficiency, typically used for final builds.

These results provide insights into the performance trade-offs of different design patterns in financial data processing systems.

## Benchmark Details

The benchmarks test the speed of pricing on 10,000 samples with 10,000 iterations, providing a comprehensive analysis of the performance of each design pattern.

## Performance Comparison Table

| Design Pattern                  | Clang++ O2 (ns/iter) | G++ O1 (ns/iter) | MSVC Release (ns/iter) |
|---------------------------------|----------------------|------------------|------------------------|
| Virtual function                | 3.46439              | 2.40862          | 2.38957                |
| Virtual Function with Pricer    | 3.59468              | 2.68404          | 2.34695                |
| Fat interface Virtual           | 3.55187              | 2.05151          | 2.33722                |
| Fat Interface with Pricer       | 3.59062              | 3.28035          | 2.68001                |
| CRTP with variant               | 1.8496               | 0.31525          | 0.322708               |
| CRTP with Pricer                | 1.84175              | 0.31589          | 0.368556               |
| Derived pricer no virtual function | 1.83058           | 0.4799           | 0.41089                |
| Dynamic cast with Pricer        | 8.553               | 6.08646          | 9.86839                |
| Static cast with Pricer         | 2.49518             | 3.01085          | 2.53508                |
| Dynamic cast in subpricer       | 7.89511             | 4.73714          | 7.66831                |
| Static cast in subpricer        | 3.50416             | 2.699            | 2.48798                |

This table provides a quick comparison of the performance across different platforms and optimization levels.