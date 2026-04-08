#!/usr/bin/env bash
set -e

echo "📊 Generate Coverage-Report..."

rm -rf coverage
mkdir coverage
gcovr \
    -r . \
    --object-directory build \
    --filter "src" \
    --exclude "tests" \
    --exclude "googletest" \
    --html --html-details \
    -o coverage/coverage.html

echo "✅ Coverage-Report generated: coverage/coverage.html"