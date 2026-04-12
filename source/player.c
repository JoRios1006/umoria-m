/* source/player.c: player specific variable definitions

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

#include "config.h"
#include "constant.h"
#include "types.h"

/* Player record for most player related info */
player_type py;
/* player location in dungeon */
int16 char_row;
int16 char_col;
/* calculated base hp values for player at each level, store them so that
   drain life + restore life does not affect hit points */
int16u player_hp[MAX_PLAYER_LEVEL];

/* Class titles for different levels				*/

/* Base experience levels, may be adjusted up for race and/or class*/
int32u player_exp[MAX_PLAYER_LEVEL] = {
      10,      25,	45,	 70,	  100,	    140,      200,	280,
     380,     500,     650,	850,	 1100,	   1400,     1800,     2300,
    2900,    3600,    4400,    5400,	 6800,	   8400,    10200,    12500,
   17500,   25000,  35000L,  50000L,   75000L,	100000L,  150000L,  200000L,
 300000L, 400000L, 500000L, 750000L, 1500000L, 2500000L, 5000000L, 10000000L
};

/*Race	STR,INT,WIS,DEX,CON,CHR,
	Ages, heights, and weights (male then female)
	Racial Bases for: dis,srh,stl,fos,bth,bthb,bsav,hitdie,
	infra, exp base, choice-classes */

/* Background information					*/

/* Classes.							*/
class_type class[MAX_CLASS] = {
/*	  HP Dis Src Stl Fos bth btb sve S  I  W  D Co Ch  Spell Exp  spl */
{"Warrior",9, 25, 14, 1, 38, 70, 55, 18, 5,-2,-2, 2, 2,-1, NONE,    0, 0},
{"Mage",   0, 30, 16, 2, 20, 34, 20, 36,-5, 3, 0, 1,-2, 1, MAGE,   30, 1},
{"Priest", 2, 25, 16, 2, 32, 48, 35, 30,-3,-3, 3,-1, 0, 2, PRIEST, 20, 1},
{"Rogue",  6, 45, 32, 5, 16, 60, 66, 30, 2, 1,-2, 3, 1,-1, MAGE,    0, 5},
{"Ranger", 4, 30, 24, 3, 24, 56, 72, 30, 2, 2, 0, 1, 1, 1, MAGE,   40, 3},
{"Paladin",6, 20, 12, 1, 38, 68, 40, 24, 3,-3, 1, 0, 2, 2, PRIEST, 35, 1}
};

/* making it 16 bits wastes a little space, but saves much signed/unsigned
   headaches in its use */
/* CLA_MISC_HIT is identical to CLA_SAVE, which takes advantage of
   the fact that the save values are independent of the class */
int16 class_level_adj[MAX_CLASS][MAX_LEV_ADJ] = {
/*	       bth    bthb   device  disarm   save/misc hit  */
/* Warrior */ {	4,	4,	2,	2,	3 },
/* Mage    */ { 2,	2,	4,	3,	3 },
/* Priest  */ { 2,	2,	4,	3,	3 },
/* Rogue   */ { 3,	4,	3,	4,	3 },
/* Ranger  */ { 3,	4,	3,	3,	3 },
/* Paladin */ { 3,	3,	3,	2,	3 }
};

int32u spell_learned = 0;	/* bit mask of spells learned */
int32u spell_worked = 0;	/* bit mask of spells tried and worked */
int32u spell_forgotten = 0;	/* bit mask of spells learned but forgotten */
int8u spell_order[32];		/* order spells learned/remembered/forgotten */

/* Warriors don't have spells, so there is no entry for them.  Note that
   this means you must always subtract one from the py.misc.pclass before
   indexing into magic_spell[]. */

char *spell_names[62] = {
  /* Mage Spells */
  "Magic Missile",  "Detect Monsters",	"Phase Door",  "Light Area",
  "Cure Light Wounds",	"Find Hidden Traps/Doors",  "Stinking Cloud",
  "Confusion",	"Lightning Bolt",  "Trap/Door Destruction", "Sleep I",
  "Cure Poison",  "Teleport Self",  "Remove Curse",  "Frost Bolt",
  "Turn Stone to Mud",	"Create Food",	"Recharge Item I",  "Sleep II",
  "Polymorph Other",  "Identify",  "Sleep III",	 "Fire Bolt",  "Slow Monster",
  "Frost Ball",	 "Recharge Item II", "Teleport Other",	"Haste Self",
  "Fire Ball", "Word of Destruction", "Genocide",
  /* Priest Spells, start at index 31 */
  "Detect Evil",  "Cure Light Wounds",	"Bless",  "Remove Fear", "Call Light",
  "Find Traps",	 "Detect Doors/Stairs",	 "Slow Poison",	 "Blind Creature",
  "Portal",  "Cure Medium Wounds",  "Chant",  "Sanctuary",  "Create Food",
  "Remove Curse",  "Resist Heat and Cold",  "Neutralize Poison",
  "Orb of Draining",  "Cure Serious Wounds",  "Sense Invisible",
  "Protection from Evil",  "Earthquake",  "Sense Surroundings",
  "Cure Critical Wounds",  "Turn Undead",  "Prayer",  "Dispel Undead",
  "Heal",  "Dispel Evil",  "Glyph of Warding",	"Holy Word"
};

/* Each type of character starts out with a few provisions.	*/
/* Note that the entries refer to elements of the object_list[] array*/
/* 344 = Food Ration, 365 = Wooden Torch, 123 = Cloak, 318 = Beginners-Majik,
   103 = Soft Leather Armor, 30 = Stiletto, 322 = Beginners Handbook */

int16u player_init[MAX_CLASS][5] = {
		{ 344, 365, 123,  30, 103},	/* Warrior	 */
		{ 344, 365, 123,  30, 318},	/* Mage		 */
		{ 344, 365, 123,  30, 322},	/* Priest	 */
		{ 344, 365, 123,  30, 318},	/* Rogue	 */
		{ 344, 365, 123,  30, 318},	/* Ranger	 */
		{ 344, 365, 123,  30, 322}	/* Paladin	 */
};


