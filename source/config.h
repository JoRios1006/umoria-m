/* source/config.h: configuration definitions

   Copyright (C) 1989-2008 James E. Wilson, Robert A. Koeneke,
                           David J. Grabiner

   This file is part of Umoria.

   Umoria is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Umoria is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Umoria.  If not, see <http://www.gnu.org/licenses/>. */

#define CONFIG_H_INCLUDED

/* Person to bother if something goes wrong. */
/* Recompile files.c and misc2.c if this changes. */
#define WIZARD  "David Grabiner <grabiner@alumni.princeton.edu>"

/* Linux build */
#define DEBIAN_LINUX
#define USG

/* This sets the default user interface. */
/* To use the original key bindings (keypad for movement) set ROGUE_LIKE
   to FALSE; to use the rogue-like key bindings (vi style movement)
   set ROGUE_LIKE to TRUE. */
/* If you change this, you only need to recompile main.c. */
#define ROGUE_LIKE FALSE

/* Files used by moria — set at compile time via -DLIBDIR="..." */
/* Default path if not overridden: */
#ifndef LIBDIR
#define LIBDIR "/usr/local/share/moria"
#endif

#define MORIA_TOP     LIBDIR "/scores.dat"
#define MORIA_WELCOME LIBDIR "/welcome.hlp"
#define MORIA_HELP LIBDIR "/moria.hlp"
#define MORIA_ORIG LIBDIR "/origmoria.hlp"
#define MORIA_MAN  LIBDIR "/moria_man.hlp"
#define MORIA_HORA LIBDIR "/hours"
#define MORIA_NEWS LIBDIR "/news"
#define MORIA_CRDT LIBDIR "/COPYING"

/* Use SYS V (USG) style string/terminal functions */
#define index  strchr
#define rindex strrchr
