cmake_minimum_required(VERSION 3.12)
project(design_patterns_benchmark CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-O1")

# List all benchmark executables
set(BENCHMARKS
    virtual_function
    virtual_pricer
    fat_interface
    fat_interface_pricer
    crtp
    crtp_pricer
    dynamic_cast_pricer
    static_cast_pricer
    derived_pricer
    dynamic_subpricer
    static_subpricer
)

foreach(target IN LISTS BENCHMARKS)
    add_executable(${target} ${target}.cpp)
endforeach() 