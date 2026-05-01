# Umoria 5.6 - Classic Roguelike Game

## Overview
Umoria (Universal Moria) is a classic terminal-based roguelike dungeon exploration game based on the original Moria (1983). Written in C (gnu89), using ncurses for terminal rendering. The codebase is Linux-only: all dead platform compatibility code (Mac, MSDOS, VMS, Atari, Amiga, etc.) has been stripped.

## Tech Stack
- **Language**: C (gnu89 standard)
- **Terminal Library**: ncurses
- **Build System**: GNU Make (multi-file) or direct gcc (monolith)

## Project Structure
- `moria.c` - **Monolithic single-file build** (~30,000 lines, all sources concatenated)
- `source/` - Core game logic (all .c and .h files)
- `unix/` - Unix/Linux platform driver and build directory (symlinks to source/)
- `files/` - Game data files (help, news, hours, scores, COPYING)
- `doc/` - Documentation and man pages
- `build/` - Debian packaging metadata
- `util/` - Developer utilities

## Building

### Monolithic single-file build (any Linux machine, no build system needed)
```bash
gcc -O2 -std=gnu89 \
    -DLIBDIR='"/home/runner/workspace/.local/share/moria"' \
    -I unix -I source \
    -Wno-implicit-function-declaration -Wno-int-conversion \
    moria.c -o moria -lncurses -ltinfo
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
- **Data paths**: `source/config.h` (MORIA_TOP, MORIA_HELP, etc.) — all override-able via `-DLIBDIR`
- **Paths point to**: `/home/runner/workspace/.local/share/moria/`

## Workflow
- **Name**: Start application
- **Command**: `TERM=xterm-256color /home/runner/.local/bin/moria`
- **Type**: console TUI (ncurses roguelike — no HTTP port)

## Key Fixes Applied

### Dead-code cleanup (orphan #endif fixer)
- Removed all platform-specific branches (MAC, MSDOS, VMS, ATARI_ST, AMIGA, etc.)
- Fixed cascading strip bugs in: death.c, dungeon.c, files.c, generate.c, io.c, main.c, misc1.c, misc3.c, save.c, signals.c, unix.c

### Restored stripped data arrays
- `c_list[]` in monsters.c (monster templates)
- `object_list[]` in treasure.c (978 items)
- `colors/mushrooms/woods/metals/rocks/amulets/syllables[]` in tables.c
- `owners[]` in tables.c (18 store owners)
- `player_title[]`, `race[]`, `background[]`, `magic_spell[]` in player.c

### save.c
- Restored `#define DEBUG(x)` (was inside stripped `#if 0` block)
- Restored `else if ((fd = open(savefile, O_RDONLY, 0)) < 0 ...)` condition before error print
- Added all static forward declarations: wr_byte, wr_short, wr_long, wr_bytes, wr_string, wr_shorts, wr_item, wr_monster, rd_byte, rd_short, rd_long, rd_bytes, rd_string, rd_shorts, rd_item, rd_monster

### io.c
- Restored `if (initscr() == NULL)` condition (was stripped, causing unconditional crash)
- Restored `#define use_value2` (empty no-op macro for non-lint builds)
- Fixed shell_out() for Linux (fork/exec/wait pattern with tcgetattr/tcsetattr)

### files.c
- Restored `init_scorefile()` body: try `fopen(MORIA_TOP, "r+")`, fall back to `"w+"` for creation
- config.h: changed MORIA_TOP from `"/scores.dat"` to `"/scores"` to match Linux convention

### externs.h (monolith compatibility)
- Added `#ifndef MORIA_EXTERNS_H` include guard
- Removed `#define open topen` and `#define fopen tfopen` macros (break monolith due to ordering)
- Fixed `extern char *copyright[5]` → `[17]` to match variable.c definition
- Fixed `int set_large(inven_type *)` → `int set_large(treasure_type *)` to match sets.c definition

### All moria headers
- Added include guards to config.h, constant.h, types.h, externs.h, local_paths.h
