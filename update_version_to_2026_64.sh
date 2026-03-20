#!/usr/bin/env bash
set -euo pipefail

NEW_VER="0.8.30.2026.64"

echo "Updating repo version strings to $NEW_VER ..."
echo

# Files that should be edited directly
sed -i "s/E-UAE 0\.8\.29-WIP4/E-UAE $NEW_VER/g" README README.md
sed -i "s/0\.8\.29-WIP4/$NEW_VER/g" src/od-macosx/Info.plist
sed -i "s/AC_INIT(E-UAE, 0\.8\.29-WIP4, ,e-uae)/AC_INIT(E-UAE, $NEW_VER, ,e-uae)/" configure.in

# Regenerate configure from configure.in if autoconf exists
if command -v autoconf >/dev/null 2>&1; then
  echo "Regenerating configure with autoconf..."
  autoconf
else
  echo "autoconf not found, patching configure directly..."
  sed -i "s/0\.8\.29-WIP4/$NEW_VER/g" configure
fi

echo
echo "Remaining old-version matches:"
grep -RIniE --exclude-dir=.git '0\.8\.29[ ._-]*WIP[ ._-]*4|0\.8\.29[ ._-]*wip[ ._-]*4' . || true
