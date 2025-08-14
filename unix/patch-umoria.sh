#!/usr/bin/env bash
# patch-umoria.sh — make UMoria build & install cleanly without sudo on modern Linux
# - Sets BINDIR/LIBDIR to ~/.local
# - Removes chown/chgrp from install rules
# - Replaces hardcoded /home/dgrabiner/... data paths in config.h
# - Patches io.c to stop using stdscr internals (uses getyx + mvcur)
# - Ensures <curses.h> is included
# - Adds -lncurses to LIBS if missing

set -euo pipefail

# --- config ---
BIN="${BIN:-$HOME/.local/bin}"
LIB="${LIB:-$HOME/.local/share/moria}"
MAKEFILES=("Makefile" "Makefile.unix")

echo "Using:"
echo "  BINDIR = $BIN"
echo "  LIBDIR = $LIB"
mkdir -p "$BIN" "$LIB"

cp ../source/* .
# --- helper: sed in-place only if file exists ---
sedi() { # usage: sedi <file> <sed-script>
  local f="$1"; shift
  [[ -f "$f" ]] || return 0
  sed -i "$@" "$f"
}

# --- 1) Patch Makefiles (both if present) ---
for mf in "${MAKEFILES[@]}"; do
  [[ -f "$mf" ]] || continue

  # Set install paths
  sedi "$mf" -e "s|^BINDIR *=.*|BINDIR = $BIN|"
  sedi "$mf" -e "s|^LIBDIR *=.*|LIBDIR = $LIB|"

  # Prefer current user/group (but also remove chown/chgrp below)
  if id -un >/dev/null 2>&1; then
    sedi "$mf" -e "s|^OWNER *=.*|OWNER = $(id -un)|"
  fi
  if id -gn >/dev/null 2>&1; then
    sedi "$mf" -e "s|^GROUP *=.*|GROUP = $(id -gn)|"
  fi

  # Remove ownership changing (no sudo installs)
  sedi "$mf" -e "/^[[:space:]]*chown[[:space:]]/d"
  sedi "$mf" -e "/^[[:space:]]*chgrp[[:space:]]/d"

  # Ensure ncurses is linked (adds if not present)
  if grep -qE '^[[:space:]]*LIBS[[:space:]]*=' "$mf"; then
    if ! grep -Eq '^[[:space:]]*LIBS[[:space:]]*=.*-lncurses' "$mf"; then
      sedi "$mf" -e 's/^\([[:space:]]*LIBS[[:space:]]*=\)/\1 -lncurses/'
    fi
  else
    # add a LIBS line near the top if none exists
    awk 'NR==1{print "LIBS = -lncurses"}1' "$mf" > "$mf.tmp" && mv "$mf.tmp" "$mf"
  fi
done

# --- 2) Replace hardcoded data paths in config.h ---
if [[ -f config.h ]]; then
  sedi config.h -e "s|/home/dgrabiner/moria-5.6/files|$LIB|g"
fi

# --- 3) Patch io.c: mvcur(stdscr->_cury, stdscr->_curx, LINES-1, 0); ---
if [[ -f io.c ]]; then
  # Add <curses.h> if missing
  if ! grep -q '^#include <curses.h>' io.c; then
    # Insert after the last existing #include line
    awk '
      BEGIN{done=0}
      /^#include /{last=NR}
      {lines[NR]=$0}
      END{
        for(i=1;i<=NR;i++){
          print lines[i]
          if(i==last && done==0){print "#include <curses.h>"; done=1}
        }
      }' io.c > io.c.tmp && mv io.c.tmp io.c
  fi

  # Replace the internal stdscr access with getyx + mvcur
  # (GNU bash $'...' quoting used to embed newlines)
  sed -i $'s|mvcur(stdscr->_cury, stdscr->_curx, LINES-1, 0);|int y, x;\\n  getyx(stdscr, y, x);\\n  mvcur(y, x, LINES-1, 0);|' io.c
fi

# --- 4) Make sure runtime files exist with sane perms ---
touch "$LIB/scores" "$LIB/hours" "$LIB/news" || true
chmod 644 "$LIB/scores" || true
chmod 444 "$LIB/hours" "$LIB/news" || true

echo "✅ Patch complete."
echo "Next steps:"
echo "  make clean && make"
echo "  make install   # now installs to $BIN and $LIB without sudo"
echo "Run: $BIN/moria"

