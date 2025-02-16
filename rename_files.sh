#!/bin/bash

# Design 1-11 renaming
mv design1_virtual.cpp virtual_function.cpp
mv design2_dynamic_cast.cpp dynamic_cast_pricer.cpp
mv design3_static_cast.cpp static_cast_pricer.cpp 
mv design4_derived_pricer.cpp derived_pricer.cpp
mv design5_fat_interface.cpp fat_interface.cpp
mv design6_crtp.cpp crtp.cpp
mv design7_dynamic_cast_in_subpricer.cpp dynamic_subpricer.cpp
mv design8_static_cast_in_subpricer.cpp static_subpricer.cpp
mv design9_virtual_with_pricer.cpp virtual_pricer.cpp
mv design10_fat_interface_with_pricer.cpp fat_interface_pricer.cpp
mv design11_crtp_with_pricer.cpp crtp_pricer.cpp

echo "Renaming complete! Verify files:"
ls -l *.cpp 