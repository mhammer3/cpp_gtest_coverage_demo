#!/usr/bin/env bash
set -e

echo "🧹 Entferne altes build-Verzeichnis..."
rm -rf build

echo "🔧 Starte CMake-Konfiguration (Debug + Coverage)..."
cmake -G "MinGW Makefiles" -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON

echo "🛠 Baue das Projekt..."
cmake --build build

echo "▶️ Führe Unit-Tests aus..."
ctest --test-dir build --output-on-failure

echo "📊 Erzeuge Coverage-Report..."
gcovr \
    --root "$(pwd)" \
    --object-directory "$(pwd)/build" \
    --filter "$(pwd)/src" \
    --exclude "$(pwd)/tests" \
    --exclude "$(pwd)/googletest" \
    --html --html-details \
    -o build/coverage.html

echo "✅ Fertig!"
echo "👉 Coverage-Datei: build/coverage.html"