#!/bin/bash

# Navigate to the build directory
cd build || exit

# Clean make
make clean

# Run CMake and Make
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=YES ..
make

ln -sf $(pwd)/compile_commands.json ../compile_commands.json

# Run the application
./shapegl

# Navigate back to the root directory
cd ..

