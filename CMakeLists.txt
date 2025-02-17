cmake_minimum_required(VERSION 3.12)
project(design_patterns_benchmark CXX)

# Ensure C++20 is used
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Properly handle optimization flags for MSVC
if (MSVC)
    # Ensure MSVC explicitly uses C++20
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")
    
    # Preserve existing flags when setting release/debug mode
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Zi") # Debug mode with debugging symbols
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

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
    derived_pricer_no_virtual
    derived_pricer_with_virtual_used
    derived_pricer_with_virtual_unused
    dynamic_subpricer
    static_subpricer
)

# Add each executable
foreach(target IN LISTS BENCHMARKS)
    add_executable(${target} ${target}.cpp)
endforeach()

# Debugging: Print out the final CXX flags to confirm they include /std:c++20
message(STATUS "CXX Flags: ${CMAKE_CXX_FLAGS}")
