#!/usr/bin/env bash
set -e

echo "🛠 Entferne altes build-Verzeichnis..."
rm -rf build

echo "🔧 Erzeuge CMake Build (Debug + Coverage)..."
cmake -G "MinGW Makefiles" -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON

echo "📦 Baue das Projekt..."
cmake --build build

echo "✅ Build erfolgreich!"