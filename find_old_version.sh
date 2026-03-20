#!/usr/bin/env bash
set -euo pipefail

ROOT="${1:-.}"
PATTERN='0\.8\.29-wip4'

echo "Searching in: $ROOT"
echo

if command -v rg >/dev/null 2>&1; then
  rg -n -i --hidden \
    -g '!/.git' \
    -g '!/build' \
    -g '!/dist' \
    -g '!/*.o' \
    -g '!/*.a' \
    -g '!/*.so' \
    -g '!/*.dll' \
    -g '!/*.exe' \
    -g '!/*.zip' \
    "$PATTERN" "$ROOT"
else
  grep -RIniE \
    --exclude-dir=.git \
    --exclude-dir=build \
    --exclude-dir=dist \
    --exclude='*.o' \
    --exclude='*.a' \
    --exclude='*.so' \
    --exclude='*.dll' \
    --exclude='*.exe' \
    --exclude='*.zip' \
    "$PATTERN" "$ROOT"
fi
