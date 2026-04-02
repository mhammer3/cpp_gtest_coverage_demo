#!/usr/bin/env bash
set -e

echo "▶️  Führe Unit-Tests aus..."
ctest --test-dir build --output-on-failure

echo "✅ Tests abgeschlossen!"
``