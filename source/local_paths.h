#ifndef LOCAL_PATHS_H
#define LOCAL_PATHS_H

#ifndef LIBDIR
#define LIBDIR "/dev/null" /* will be replaced via -DLIBDIR="..." in CPPFLAGS */
#endif

/* Nuke legacy/conflicting defines and force our LIBDIR-based paths */
#undef MORIA_HOU
#undef MORIA_MOR
#undef MORIA_GPL
#undef MORIA_TOP
#undef MORIA_HELP
#undef MORIA_ORIG_HELP
#undef MORIA_WIZ_HELP
#undef MORIA_OWIZ_HELP
#undef MORIA_WELCOME
#undef MORIA_VER

#define MORIA_HOU        LIBDIR "/hours"
#define MORIA_MOR        LIBDIR "/news"
#define MORIA_GPL        LIBDIR "/COPYING"
#define MORIA_TOP        LIBDIR "/scores"
#define MORIA_HELP       LIBDIR "/roglcmds.hlp"
#define MORIA_ORIG_HELP  LIBDIR "/origcmds.hlp"
#define MORIA_WIZ_HELP   LIBDIR "/rwizcmds.hlp"
#define MORIA_OWIZ_HELP  LIBDIR "/owizcmds.hlp"
#define MORIA_WELCOME    LIBDIR "/welcome.hlp"
#define MORIA_VER        LIBDIR "/version.hlp"

#endif /* LOCAL_PATHS_H */

