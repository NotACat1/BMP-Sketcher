#!/bin/bash
# Exit immediately if a command fails
set -e

# Move to project root (assumes script is in ./scripts/)
cd "$(dirname "$0")/.."

BUILD_DIR="build"

# Check if the build directory exists
if [ ! -d "$BUILD_DIR" ]; then
  echo "Build directory not found. Run scripts/build.sh first."
  exit 1
fi

# Run unit tests with output on failure
cd "$BUILD_DIR"
ctest --output-on-failure

# Optionally run benchmarks if the executable exists
if [ -f "./BMP_Sketcher_benchmarks" ]; then
  echo "Running benchmarks..."
  ./BMP_Sketcher_benchmarks
else
  echo "Benchmark executable not found. To build it, run with -DBUILD_BENCHMARKS=ON"
fi
