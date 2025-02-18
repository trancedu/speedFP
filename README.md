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

## UML Class Diagram in general 
Note: Code in some files have some differences, for example the pricer may call derived Data pointer, or the pricer is a derived pointer, etc.

```plaintext
+------------------+
|      Data        |
+------------------+
| - commonFactor   |
| - pricer         |
+------------------+
       ^
       |
+------------------+   +------------------+
|   StockData      |   |   OptionData     |
+------------------+   +------------------+
| - priceFactor    |   | - volatility     |
+------------------+   +------------------+
       
+------------------+
|     Pricer       |
+------------------+
| + calculatePrice(Data*) |
+------------------+
       ^
       |
+------------------+   +------------------+
|   StockPricer    |   |   OptionPricer   |
+------------------+   +------------------+
|                  |   |                  |
+------------------+   +------------------+
```

### Key Points

- **Data**: 
  - Base class with data members `commonFactor` and `pricer`.
  - Inherited by `StockData` and `OptionData`.

- **StockData**:
  - Inherits from `Data` and includes the data member `priceFactor`.

- **OptionData**:
  - Inherits from `Data` and includes the data member `volatility`.

- **Pricer**:
  - Base class with the method `calculatePrice(Data*)`.
  - Inherited by `StockPricer` and `OptionPricer`.

- **StockPricer** and **OptionPricer**:
  - Both inherit from `Pricer`.

This diagram now includes the `calculatePrice(Data*)` method in the `Pricer` class, along with the specified data members.
### Different Designs in the code
Note, some designs in the code are different, for example they may use derived pricer pointer, or the pricer calls derived Data pointer, the UML is just a general idea of the inheritance hierarchy.

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
| Virtual function                | 3.47503              | 2.10989          | 2.61694                |
| Virtual Function with Pricer    | 3.45394              | 2.7557           | 2.41203                |
| Fat interface Virtual           | 3.4543               | 2.7318           | 2.91102                |
| Fat Interface with Pricer       | 3.45567              | 3.35418          | 2.95541                |
| CRTP with variant               | 1.72639              | 0.39919          | 0.40957                |
| CRTP with Pricer                | 1.71321              | 0.37822          | 0.486885               |
| Derived pricer no virtual function | 1.7154            | 0.36884          | 0.45207               |
| Derived pricer with virtual used | 1.71292             | 1.78842          | 1.97573                |
| Derived pricer with virtual unused | 1.73015           | 1.57014          | 1.94696                |
| Dynamic cast with Pricer        | 8.553               | 6.08646          | 11.0482                |
| Static cast with Pricer         | 2.49518             | 3.01085          | 2.63515                |
| Dynamic cast in subpricer       | 7.89511             | 4.73714          | 8.04488                |
| Static cast in subpricer        | 3.50416             | 2.699            | 2.92021                |

This table provides a quick comparison of the performance across different platforms and optimization levels.

# Some Conclusions
These are just my observations, I may be wrong.
1. Virtual function has significant overhead, compared to non virtual function or CRTP.
2. Dynamic cast has even more overhead, if possible, maintain a enum of types and use static cast.
3. Theoretically, in Object Oriented Design, data should be bundled with the business logic, so that down casting is not needed. If you have to separate data and business logic, you will not be able to get the subclass data members without down casting, so you need to either choose CRTP, or fat interface with virtual function to get subclass information or static/dynamic cast to get subclass pointer. CRTP is the fastest, fat interface or static casts is slower and uglier, dynamic cast is the slowest.
4. In G++ and MSVC, even virtual function is not used in the derived class, it is still much slower than non virtual function and creates overhead.