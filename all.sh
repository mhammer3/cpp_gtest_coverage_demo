#!/usr/bin/env bash
set -e

echo "🧹 Remove old build-Directories..."
rm -rf build
rm -rf coverage
rm -rf clang-tidy

echo "🔧 Start CMake-Configuration (Debug + Coverage)..."
cmake -G "MinGW Makefiles" -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON

echo "🛠 Build project..."
cmake --build build

echo "▶️ Execute Unit-Tests ..."
ctest --test-dir build --output-on-failure

echo "📊 Erzeuge Coverage-Report..."
mkdir coverage
gcovr \
    -r . \
    --object-directory build \
    --filter "src" \
    --exclude "tests" \
    --exclude "googletest" \
    --html --html-details \
    -o coverage/coverage.html

echo "🔍 Run clang-tidy"
mkdir -p clang-tidy
clang-tidy src/*.c \
    -p build \
    -checks=clang-analyzer-*,bugprone-*,performance-* \
    -header-filter='^./src/.*' \
    -export-fixes=clang-tidy/fixes.yaml \
    > clang-tidy/clang-tidy-report.txt

echo "✅ Complete!"
echo "👉 Coverage-Report:coverage/coverage.html"
echo "👉 Clang-tidy-Report:clang-tidy/clang-tidy-report.txt"