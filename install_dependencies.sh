#!/bin/bash
# Exit on any failure
set -e

# Update package list
echo "Updating package list..."
sudo apt update

# Install essential build tools and CMake
echo "Installing build tools and CMake..."
sudo apt install -y build-essential cmake

# Install OpenMP and OpenCL development libraries
echo "Installing OpenMP, OpenCL, and threads..."
sudo apt install -y libomp-dev ocl-icd-opencl-dev

# Install GoogleTest and Google Benchmark libraries
echo "Installing GoogleTest and Benchmark libraries..."
sudo apt install -y libgtest-dev libbenchmark-dev

# GoogleTest must be built manually from source
echo "Building GoogleTest..."
cd /usr/src/gtest
sudo cmake .
sudo make
sudo cp lib/*.a /usr/lib

echo "All dependencies installed."
