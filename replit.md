# Umoria 5.6 - Classic Roguelike Game

## Overview
Umoria (Universal Moria) is a classic terminal-based roguelike dungeon exploration game based on the original Moria (1983). Written in C, using ncurses for terminal rendering.

## Tech Stack
- **Language**: C (gnu89 standard)
- **Terminal Library**: ncurses
- **Build System**: GNU Make (multi-file) or direct gcc (monolith)

## Project Structure
- `moria.c` - **Monolithic single-file build** (36,740 lines, all sources concatenated)
- `source/` - Core game logic (all .c and .h files)
- `unix/` - Unix/Linux platform driver and build directory (symlinks to source/)
- `files/` - Game data files (help, news, hours, scores, COPYING)
- `doc/` - Documentation and man pages
- `build/` - Debian packaging metadata
- `util/` - Developer utilities

## Building

### Monolithic single-file build (any Linux machine, no build system needed)
```bash
gcc moria.c -lncurses -DLIBDIR=\"$HOME/.local/share/moria\" -std=gnu89 \
    -Wno-implicit-function-declaration -Wno-int-conversion -o moria
```
Then copy the `files/` directory to `$HOME/.local/share/moria/`.

### Multi-file build (Replit / development)
```bash
cd unix
make
make install
```

## Game Data
- Data files live at: `/home/runner/workspace/.local/share/moria/` (Replit) or `~/.local/share/moria/` (other machines)
- Binary installed to: `/home/runner/.local/bin/moria`

## Configuration
- **Data paths**: hardcoded in `source/config.h` (Generic UNIX section, lines ~244-256)
- **Paths point to**: `/home/runner/workspace/.local/share/moria/`

## Workflow
- **Name**: Start application
- **Command**: `/home/runner/.local/bin/moria`
- **Type**: console (TUI application)

## Fixes Applied

### Source fixes (source files — required before regenerating moria.c)
1. **Buffer overflow**: `days[7][29]` → `days[7][82]` in `source/tables.c` and `source/externs.h` — fgets includes newline, old buffer was too small.
2. **Hardcoded developer paths**: `source/config.h` — added `#ifndef LIBDIR` guards so the `LIBDIR` compile-time flag wins.
3. **`int bool` conflict**: `source/misc3.c` function `mmove()` — renamed local variable `bool` → `moria_flag` (ncurses defines `bool` as a type).
4. **`getuid`/`getgid` redeclarations**: `source/death.c`, `source/main.c` — wrapped with `#ifndef __linux__` since `<unistd.h>` already provides these on Linux.
5. **Static `flock` hack conflict**: `source/death.c` — added `&& !defined(__linux__)` to the surrounding `#if` condition; Linux has `flock()` in `<sys/file.h>`.

### Build fixes (unix/Makefile)
6. `CPPFLAGS ?=` → `override CPPFLAGS +=` so `-DLIBDIR` always injects.
7. Added `-ltinfo` to `LDLIBS`.
8. Rewrote Makefile with proper tab characters (edit tool had corrupted them).

### Monolith generation
- Generator script: `/tmp/gen_monolith.sh` — concatenates all source files with system headers placed before project headers (avoids `#define open topen` and `#define fopen tfopen` conflicts from `externs.h`).
- `<signal.h>` placed before `externs.h` so the `suspend()`/`SIGTSTP` declaration inside `externs.h` resolves correctly.
