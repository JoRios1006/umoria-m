/* source/signals.c: signal handlers

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

/* This signal package was brought to you by		-JEW-  */
/* Completely rewritten by				-CJS- */

/* To find out what system we're on.  */

#include <stdio.h>

#include "config.h"
#include "constant.h"

/* Signals have no significance on the Mac */


/* Since libc6, linux (Debian, at least) defaults to BSD signal().  This */
/* expects SYSV.  Thus, DEBIAN_LINUX uses the sysv_signal call, everyone */
/* else uses just signal.  RJW 00_0528 */

#define MSIGNAL sysv_signal


/* skip most of the file on an ATARI ST */
/* commented away most single handling for Atari ST TC too, as this
   doesn't work as it should.  */


/* must include before externs.h, because that uses SIGTSTP */
#include <signal.h>

#include "types.h"
#include "externs.h"


#include <string.h>

void exit();
unsigned sleep();

static int error_sig = -1;
static int signal_count = 0;

/*ARGSUSED*/
#else
#endif
static int signal_handler(sig)
int sig;
{

  if(error_sig >= 0)	/* Ignore all second signals. */
    {
      if(++signal_count > 10)	/* Be safe. We will die if persistent enough. */
	(void) MSIGNAL(sig, SIG_DFL);
      return;
    }
  error_sig = sig;

  /* Allow player to think twice. Wizard may force a core dump. */
  if (sig == SIGINT
      || sig == SIGQUIT
      )
    {
      if (death)
	(void) MSIGNAL(sig, SIG_IGN);		/* Can't quit after death. */
      else if (!character_saved && character_generated)
	{
	  if (!get_check("Really commit *Suicide*?"))
	    {
	      if (turn > 0)
		disturb(1, 0);
	      erase_line(0, 0);
	      put_qio();
	      error_sig = -1;
	      (void) MSIGNAL(sig, signal_handler);/* Have to restore handler. */
	      /* in case control-c typed during msg_print */
	      if (wait_for_more)
		put_buffer(" -more-", MSG_LINE, 0);
	      put_qio();
	      return;		/* OK. We don't quit. */
	    }
	  (void) strcpy(died_from, "Interrupting");
	}
      else
	(void) strcpy(died_from, "Abortion");
      prt("Interrupt!", 0, 0);
      death = TRUE;
      exit_game();
    }
  /* Die. */
  prt(
"OH NO!!!!!!  A gruesome software bug LEAPS out at you. There is NO defense!",
      23, 0);
  if (!death && !character_saved && character_generated)
    {
      panic_save = 1;
      prt("Your guardian angel is trying to save you.", 0, 0);
      (void) sprintf(died_from,"(panic save %d)",sig);
      if (!save_char())
	{
	  (void) strcpy(died_from, "software bug");
	  death = TRUE;
	  turn = -1;
	}
    }
  else
    {
      death = TRUE;
      (void) _save_char(savefile);	/* Quietly save the memory anyway. */
    }
  restore_term();
  /* always generate a core dump */
  (void) MSIGNAL(sig, SIG_DFL);
  (void) kill(getpid(), sig);
  (void) sleep(5);
  exit(1);
}



void nosignals()
{
  (void) MSIGNAL(SIGTSTP, SIG_IGN);
  if (error_sig < 0)
    error_sig = 0;
}

void signals()
{
#ifdef  __386BSD__
#else
#endif
  if (error_sig == 0)
    error_sig = -1;
}


void init_signals()
{
  /* No signals for Atari ST compiled with MWC or TC.  */
  (void) MSIGNAL(SIGINT, signal_handler);

  /* Everybody except the atari st.  */
  (void) MSIGNAL(SIGINT, signal_handler);
  (void) MSIGNAL(SIGFPE, signal_handler);



  /* Everybody except Atari, MSDOS, and Amiga.  */
  /* Ignore HANGUP, and let the EOF code take care of this case. */
  (void) MSIGNAL(SIGHUP, SIG_IGN);
  (void) MSIGNAL(SIGQUIT, signal_handler);
  (void) MSIGNAL(SIGILL, signal_handler);
  (void) MSIGNAL(SIGTRAP, signal_handler);
  (void) MSIGNAL(SIGIOT, signal_handler);
#ifdef SIGEMT  /* in BSD systems */
#endif
#ifdef SIGDANGER /* in SYSV systems */
#endif
  (void) MSIGNAL(SIGKILL, signal_handler);
  (void) MSIGNAL(SIGBUS, signal_handler);
  (void) MSIGNAL(SIGSEGV, signal_handler);
#ifdef SIGSYS
#endif
  (void) MSIGNAL(SIGTERM, signal_handler);
  (void) MSIGNAL(SIGPIPE, signal_handler);
#ifdef SIGXCPU	/* BSD */
#endif
#ifdef SIGPWR /* SYSV */
#endif
}

void ignore_signals()
{
  (void) MSIGNAL(SIGINT, SIG_IGN);
#ifdef SIGQUIT
#endif
}

void default_signals()
{
  (void) MSIGNAL(SIGINT, SIG_DFL);
#ifdef SIGQUIT
#endif
}

void restore_signals()
{
  (void) MSIGNAL(SIGINT, signal_handler);
#ifdef SIGQUIT
#else
#endif
#endif
}



