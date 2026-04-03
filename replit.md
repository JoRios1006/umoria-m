# Umoria 5.6 - Classic Roguelike Game

## Overview
Umoria (Universal Moria) is a classic terminal-based roguelike dungeon exploration game based on the original Moria (1983). Written in C, using ncurses for terminal rendering.

## Tech Stack
- **Language**: C (gnu89 standard)
- **Terminal Library**: ncurses
- **Build System**: GNU Make

## Project Structure
- `source/` - Core game logic (all .c and .h files)
- `unix/` - Unix/Linux platform driver and build directory (symlinks to source/)
- `files/` - Game data files (help, news, hours, scores, COPYING)
- `doc/` - Documentation and man pages
- `build/` - Debian packaging metadata
- `util/` - Developer utilities

## Build Instructions
The build compiles from the `unix/` directory which has symlinks to all source files:

```bash
cd unix
make          # build
make install  # install to ~/.local/bin and ~/.local/share/moria
```

## Game Data
Installed to: `/home/runner/workspace/.local/share/moria/`
Binary installed to: `/home/runner/.local/bin/moria`

## Configuration
- **Data paths**: hardcoded in `source/config.h` (Generic UNIX section, lines ~244-256)
- **Paths point to**: `/home/runner/workspace/.local/share/moria/`
- **Key fix applied**: `days[7][82]` buffer in `source/tables.c` and `source/externs.h` (was 29, caused buffer overflow with newline-terminated hours file lines)

## Workflow
- **Name**: Start application
- **Command**: `TERM=xterm-256color /home/runner/.local/bin/moria`
- **Type**: console (TUI application)

## Known Issues Fixed
1. **Buffer overflow**: `days[7][29]` was too small for fgets lines from hours file (included newline). Fixed to `[82]`.
2. **Hardcoded paths**: `config.h` had paths from developer's machine (`/home/jo/`). Updated to `/home/runner/workspace/.local/share/moria/`.
