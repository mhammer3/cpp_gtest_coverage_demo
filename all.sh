#!/usr/bin/env bash
set -e

echo "🧹 Entferne altes build-Verzeichnis..."
rm -rf build
rm -rf coverage

echo "🔧 Starte CMake-Konfiguration (Debug + Coverage)..."
cmake -G "MinGW Makefiles" -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON

echo "🛠 Baue das Projekt..."
cmake --build build

echo "▶️ Führe Unit-Tests aus..."
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

echo "✅ Fertig!"
echo "👉 Coverage-Datei:coverage/coverage.html"