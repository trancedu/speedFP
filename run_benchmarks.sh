#!/bin/bash

# Build first
mkdir -p build && cd build
cmake .. && make -j$(nproc)

# Run in specified order
./virtual_function
./virtual_pricer
./fat_interface
./fat_interface_pricer
./crtp
./crtp_pricer
./derived_pricer_no_virtual
./derived_pricer_with_virtual_used
./derived_pricer_with_virtual_unused
./dynamic_cast_pricer
./static_cast_pricer
./dynamic_subpricer
./static_subpricer 