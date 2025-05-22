#!/bin/bash
# Exit on any error
set -e

# Move to project root (assumes script is in ./scripts/)
cd "$(dirname "$0")/.."

# Remove the build directory and all build artifacts
echo "Removing build directory..."
rm -rf build
echo "Clean complete."
