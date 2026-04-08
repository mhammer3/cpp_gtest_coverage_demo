#!/usr/bin/env bash
set -e

echo "▶️  Execute Unit-Tests..."
ctest --test-dir build --output-on-failure

echo "✅ Tests completed!"
