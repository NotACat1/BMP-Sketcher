#!/bin/bash
# Exit immediately if a command exits with a non-zero status
set -e

# Move to project root (assumes script is in ./scripts/)
cd "$(dirname "$0")/.."

# Build type defaults to Debug if not specified
BUILD_TYPE=${1:-Debug}
BUILD_DIR="build"

# Create and enter the build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure the project with CMake
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..

# Build the project using all available CPU cores
cmake --build . -- -j$(nproc)

echo "Build completed in $BUILD_TYPE mode."
