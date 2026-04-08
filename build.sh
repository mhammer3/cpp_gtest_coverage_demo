#!/usr/bin/env bash
set -e

echo "🛠 Remove old build directory..."
rm -rf build

echo "🔧 Generate CMake Build (Debug + Coverage)..."
cmake -G "MinGW Makefiles" -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON

echo "📦 Build the project..."
cmake --build build

echo "✅ Build successful!"