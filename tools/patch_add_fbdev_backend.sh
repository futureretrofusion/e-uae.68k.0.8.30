#!/bin/bash
set -e

ROOT="${1:-.}"
cd "$ROOT"

if grep -q "with-fbdev" configure.in; then
    echo "fbdev patch already applied"
    exit 0
fi

python3 <<'PY'
from pathlib import Path
p = Path('configure.in')
s = p.read_text()

s = s.replace(
'''AC_ARG_WITH(curses,\n  AS_HELP_STRING([--with-curses], [Use ncurses library for graphics]),\n  [WANT_NCURSES=$withval], [])''',
'''AC_ARG_WITH(curses,\n  AS_HELP_STRING([--with-curses], [Use ncurses library for graphics]),\n  [WANT_NCURSES=$withval], [])\n\nAC_ARG_WITH(fbdev,\n  AS_HELP_STRING([--with-fbdev], [Use Linux framebuffer graphics backend]),\n  [WANT_FBDEV=$withval], [])''')

s = s.replace(
'''dnl  If we got here and we still haven't found a graphics target''',
'''if [[ "x$WANT_FBDEV" = "xyes" ]]; then\n  GFX_DEP=gfx-fbdev\n  GFX_NAME=fbdev\n  GFX_LIBS=\n  GFX_CFLAGS=\n  GFX_CPPFLAGS=\nfi\n\ndnl  If we got here and we still haven't found a graphics target''')

s = s.replace(
'''\t\t src/gfx-sdl/Makefile\n\t\t src/gfx-curses/Makefile''',
'''\t\t src/gfx-sdl/Makefile\n\t\t src/gfx-fbdev/Makefile\n\t\t src/gfx-curses/Makefile''')

p.write_text(s)
PY

autoreconf -fi

echo "fbdev backend patch applied"
