/* source/externs.h: declarations for global variables and initialized data

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

#include<stdio.h>

/* VMS requires that this be in externs.h, not files.c; this prevents a
   'psect' error for the variable errno */
#include <errno.h>

/* Atari TC requires prototypes, but does not have __STDC__.
   Hence, we check for ATARIST_TC here, and define LINT_ARGS if true.  */

/* This causes more trouble than it is worth, and very few systems still
   have this bug in their include files.  */
#if 0
#else
#endif

/* to avoid 'psect' problem with VMS declaration of errno */
extern int errno;

extern char *copyright[5];

/* horrible hack: needed because compact_monster() can be called from deep
   within creatures() via place_monster() and summon_monster() */
extern int hack_monptr;

extern vtype died_from;
extern vtype savefile;                  /* The save file. -CJS- */
extern int32 birth_date;

/* These are options, set with set_options command -CJS- */
extern int rogue_like_commands;
extern int find_cut;                    /* Cut corners on a run */
extern int find_examine;                /* Check corners on a run */
extern int find_prself;                 /* Print yourself on a run (slower) */
extern int find_bound;                  /* Stop run when the map shifts */
extern int prompt_carry_flag;           /* Prompt to pick something up */
extern int show_weight_flag;            /* Display weights in inventory */
extern int highlight_seams;             /* Highlight magma and quartz */
extern int find_ignore_doors;           /* Run through open doors */
extern int sound_beep_flag;             /* Beep for invalid character */
extern int display_counts;              /* Display rest/repeat counts */

/* global flags */
extern int new_level_flag;        /* Next level when true  */
extern int teleport_flag;       /* Handle teleport traps  */
extern int eof_flag;            /* Used to handle eof/HANGUP */
extern int player_light;      /* Player carrying light */
extern int find_flag;   /* Used in MORIA              */
extern int free_turn_flag;      /* Used in MORIA              */
extern int weapon_heavy;        /* Flag if the weapon too heavy -CJS- */
extern int pack_heavy;          /* Flag if the pack too heavy -CJS- */
extern char doing_inven;        /* Track inventory commands */
extern int screen_change;       /* Screen changes (used in inven_commands) */

extern int character_generated;  /* don't save score until char gen finished*/
extern int character_saved;      /* prevents save on kill after save_char() */
extern FILE *highscore_fp;      /* High score file pointer */
extern int command_count;       /* Repetition of commands. -CJS- */
extern int default_dir;         /* Use last direction in repeated commands */
extern int16 noscore;           /* Don't score this game. -CJS- */
extern int32u randes_seed;    /* For encoding colors */
extern int32u town_seed;            /* Seed for town genera*/
extern int16 dun_level; /* Cur dungeon level   */
extern int16 missile_ctr;       /* Counter for missiles */
extern int msg_flag;    /* Set with first msg  */
extern vtype old_msg[MAX_SAVE_MSG];     /* Last messages -CJS- */
extern int16 last_msg;                  /* Where in the array is the last */
extern int death;       /* True if died       */
extern int32 turn;      /* Cur trun of game    */
extern int wizard;      /* Wizard flag        */
extern int to_be_wizard;
extern int16 panic_save; /* this is true if playing from a panic save */

extern int wait_for_more;

extern char days[7][82];
extern int closing_flag;        /* Used for closing   */

extern int16 cur_height, cur_width;     /* Cur dungeon size    */
/*  Following are calculated from max dungeon sizes             */
extern int16 max_panel_rows, max_panel_cols;
extern int panel_row, panel_col;
extern int panel_row_min, panel_row_max;
extern int panel_col_min, panel_col_max;
extern int panel_col_prt, panel_row_prt;

/*  Following are all floor definitions                         */
extern cave_type cave[MAX_HEIGHT][MAX_WIDTH];

/* Following are player variables                               */
extern player_type py;
extern int32u player_exp[MAX_PLAYER_LEVEL];
extern int16u player_hp[MAX_PLAYER_LEVEL];
extern int16 char_row;
extern int16 char_col;

extern int8u rgold_adj[MAX_RACES][MAX_RACES];

extern class_type class[MAX_CLASS];
extern int16 class_level_adj[MAX_CLASS][MAX_LEV_ADJ];

/* Warriors don't have spells, so there is no entry for them. */
extern char *spell_names[62];
extern int32u spell_learned;    /* Bit field for spells learnt -CJS- */
extern int32u spell_worked;     /* Bit field for spells tried -CJS- */
extern int32u spell_forgotten;  /* Bit field for spells forgotten -JEW- */
extern int8u spell_order[32];   /* remember order that spells are learned in */
extern int16u player_init[MAX_CLASS][5];
extern int16 total_winner;
extern int32 max_score;

/* Following are store definitions                              */
extern store_type store[MAX_STORES];
extern int16u store_choice[MAX_STORES][STORE_CHOICES];
extern int (*store_buy[MAX_STORES])();

/* Following are treasure arrays        and variables                   */
extern int8u object_ident[OBJECT_IDENT_SIZE];
extern int16 t_level[MAX_OBJ_LEVEL+1];
extern inven_type t_list[MAX_TALLOC];
extern inven_type inventory[INVEN_ARRAY_SIZE];
extern char *special_names[SN_ARRAY_SIZE];
extern int16 sorted_objects[MAX_DUNGEON_OBJ];
extern int16 inven_ctr;         /* Total different obj's        */
extern int16 inven_weight;      /* Cur carried weight   */
extern int16 equip_ctr; /* Cur equipment ctr    */
extern int16 tcptr;     /* Cur treasure heap ptr        */

/* Following are creature arrays and variables                  */
extern monster_type m_list[MAX_MALLOC];
extern int16 m_level[MAX_MONS_LEVEL+1];
extern m_attack_type monster_attacks[N_MONS_ATTS];
extern recall_type c_recall[MAX_CREATURES];     /* Monster memories. -CJS- */
extern monster_type blank_monster;      /* Blank monster values */
extern int16 mfptr;     /* Cur free monster ptr */
extern int16 mon_tot_mult;      /* # of repro's of creature     */

/* Following are arrays for descriptive pieces                  */

extern int8u blows_table[7][6];

extern int16u normal_table[NORMAL_TABLE_SIZE];

/* Initialized data which had to be moved from some other file */
/* Since these get modified, macrsrc.c must be able to access them */
/* Otherwise, game cannot be made restartable */
/* dungeon.c */
extern char last_command;  /* Memory of previous command. */
/* moria1.c */
/* Track if temporary light about player.  */
extern int light_flag;


/* function return values */
/* only extern functions declared here, static functions declared inside
   the file that defines them */
#if defined(LINT_ARGS)
#endif

/* call functions which expand tilde before calling open/fopen */
#define open topen
#define fopen tfopen

/* st-stuff.c for the atari ST */



