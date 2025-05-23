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
echo "Installing OpenMP, and threads..."
sudo apt install -y libomp-dev

echo "All dependencies installed."
