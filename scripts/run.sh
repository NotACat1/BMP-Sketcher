#!/bin/bash
# Exit on error
set -e

# Move to project root (assumes script is in ./scripts/)
cd "$(dirname "$0")/.."

BUILD_DIR="build"

# Ensure the application is built before running
if [ ! -f "$BUILD_DIR/BMP_Sketcher" ]; then
    echo "Executable not found. Build the project first using scripts/build.sh"
    exit 1
fi

# Run the compiled binary with any passed arguments
"$BUILD_DIR/BMP_Sketcher" "$@"
