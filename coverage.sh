#!/usr/bin/env bash
set -e

echo "📊 Erzeuge Coverage-Report..."

gcovr \
    --root "$(pwd)" \
    --object-directory "$(pwd)/build" \
    --filter "$(pwd)/src" \
    --exclude "$(pwd)/tests" \
    --exclude "$(pwd)/googletest" \
    --html --html-details \
    -o build/coverage.html

echo "✅ Coverage-Report erstellt: build/coverage.html"