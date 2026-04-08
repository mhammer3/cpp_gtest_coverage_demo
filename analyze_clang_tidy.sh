#!/usr/bin/env bash
set -e

echo "🧹 Clean"
rm -rf build clang-tidy
mkdir -p clang-tidy

echo "🔧 Configure"
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

echo "🛠 Build"
cmake --build build

echo "🔍 Run clang-tidy"
clang-tidy src/*.c \
    -p build \
    -checks=clang-analyzer-*,bugprone-*,performance-* \
    -header-filter='^./src/.*' \
    -export-fixes=clang-tidy/fixes.yaml \
    > clang-tidy/report.txt

echo "✅ Done"