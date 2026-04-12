/* source/monsters.c: monster definitions

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

/* Following are creature arrays and variables			*/
	/* Creatures must be defined here				*/
	/*	See types.h under creature_type for a complete list
		of all variables for creatures.	 Some of the less obvious
		are explained below.

	Hit points:	#1, #2: where #2 is the range of each roll and
			#1 is the number of added up rolls to make.
			Example: a creature with 5 eight-sided hit die
			is given {5,8}.

		Attack types:
		1	Normal attack
		2	Poison Strength
		3	Confusion attack
		4	Fear attack
		5	Fire attack
		6	Acid attack
		7	Cold attack
		8	Lightning attack
		9	Corrosion attack
		10	Blindness attack
		11	Paralysis attack
		12	Steal Money
		13	Steal Object
		14	Poison
		15	Lose dexterity
		16	Lose constitution
		17	Lose intelligence
		18	Lose wisdom
		19	Lose experience
		20	Aggravation
		21	Disenchants
		22	Eats food
		23	Eats light
		24	Eats charges
		99	Blank

		Attack descriptions:
		1	hits you.
		2	bites you.
		3	claws you.
		4	stings you.
		5	touches you.
		6	kicks you.
		7	gazes at you.
		8	breathes on you.
		9	spits on you.
		10	makes a horrible wail.
		11	embraces you.
		12	crawls on you.
		13	releases a cloud of spores.
		14	begs you for money.
		15	You've been slimed.
		16	crushes you.
		17	tramples you.
		18	drools on you.
		19	insults you.
		99	is repelled.

	Example:  For a creature which bites for 1d6, then stings for
		  2d4 and loss of dex you would use:
			{1,2,1,6},{15,4,2,4}

		CMOVE flags:
Movement.	00000001	Move only to attack
	.	00000002	Move, attack normal
	.	00000008	20% random movement
	.	00000010	40% random movement
	.	00000020	75% random movement
Special +	00010000	Invisible movement
	+	00020000	Move through door
	+	00040000	Move through wall
	+	00080000	Move through creatures
	+	00100000	Picks up objects
	+	00200000	Multiply monster
Carries =	01000000	Carries objects.
	=	02000000	Carries gold.
	=	04000000	Has 60% of time.
	=	08000000	Has 90% of time.
	=	10000000	1d2 objects/gold.
	=	20000000	2d2 objects/gold.
	=	40000000	4d2 objects/gold.
Special ~	80000000	Win-the-Game creature.

		SPELL Flags:
Frequency	000001	  1	These add up to x.  Then
(1 in x).	000002	  2	if RANDINT(X) = 1 the
	.	000004	  4	creature casts a spell.
	.	000008	  8
Spells	=	000010	Teleport short (blink)
	=	000020	Teleport long
	=	000040	Teleport player to monster
	=	000080	Cause light wound
	=	000100	Cause serious wound
	=	000200	Hold person (Paralysis)
	=	000400	Cause blindness
	=	000800	Cause confusion
	=	001000	Cause fear
	=	002000	Summon monster
	=	004000	Summon undead
	=	008000	Slow Person
	=	010000	Drain Mana
	=	020000	Not Used
	=	040000	Not Used
Breath/	+	080000	Breathe/Resist Lightning
Resist	+	100000	Breathe/Resist Gas
	+	200000	Breathe/Resist Acid
	+	400000	Breathe/Resist Frost
	+	800000	Breathe/Resist Fire

		CDEFENSE flags:
		0001	Hurt by Slay Dragon.
		0002	Hurt by Slay Animal.
		0004	Hurt by Slay Evil.
		0008	Hurt by Slay Undead.
		0010	Hurt by Frost.
		0020	Hurt by Fire.
		0040	Hurt by Poison.
		0080	Hurt by Acid.
		0100	Hurt by Light-Wand.
		0200	Hurt by Stone-to-Mud.
		0400	Not used.
		0800	Not used.
		1000	Cannot be charmed or slept.
		2000	Can be seen with infra-vision.
		4000	Max Hit points.
		8000	Not used.


	Sleep (sleep)	:	A measure in turns of how fast creature
				will notice player (on the average).
	Area of affect (aaf) :	Max range that creature is able to "notice"
				the player.
									*/


/* ERROR: attack #35 is no longer used */
struct m_attack_type monster_attacks[N_MONS_ATTS] = {
/* 0 */	{0, 0, 0, 0},	{1, 1, 1, 2},	{1, 1, 1, 3},	{1, 1, 1, 4},
	{1, 1, 1, 5},	{1, 1, 1, 6},	{1, 1, 1, 7},	{1, 1, 1, 8},
	{1, 1, 1, 9},	{1, 1, 1, 10},	{1, 1, 1, 12},	{1, 1, 2, 2},
	{1, 1, 2, 3},	{1, 1, 2, 4},	{1, 1, 2, 5},	{1, 1, 2, 6},
	{1, 1, 2, 8},	{1, 1, 3, 4},	{1, 1, 3, 5},	{1, 1, 3, 6},
/* 20 */{1, 1, 3, 8},	{1, 1, 4, 3},	{1, 1, 4, 6},	{1, 1, 5, 5},
	{1, 2, 1, 1},	{1, 2, 1, 2},	{1, 2, 1, 3},	{1, 2, 1, 4},
	{1, 2, 1, 5},	{1, 2, 1, 6},	{1, 2, 1, 7},	{1, 2, 1, 8},
	{1, 2, 1, 10},	{1, 2, 2, 3},	{1, 2, 2, 4},	{1, 2, 2, 5},
	{1, 2, 2, 6},	{1, 2, 2, 8},	{1, 2, 2, 10},	{1, 2, 2, 12},
/* 40 */{1, 2, 2, 14},	{1, 2, 3, 4},	{1, 2, 3, 12},	{1, 2, 4, 4},
	{1, 2, 4, 5},	{1, 2, 4, 6},	{1, 2, 4, 8},	{1, 2, 5, 4},
	{1, 2, 5, 8},	{1, 3, 1, 1},	{1, 3, 1, 2},	{1, 3, 1, 3},
	{1, 3, 1, 4},	{1, 3, 1, 5},	{1, 3, 1, 8},	{1, 3, 1, 9},
	{1, 3, 1, 10},	{1, 3, 1, 12},	{1, 3, 3, 3},	{1, 4, 1, 2},
/* 60 */{1, 4, 1, 3},	{1, 4, 1, 4},	{1, 4, 2, 4},	{1, 5, 1, 2},
	{1, 5, 1, 3},	{1, 5, 1, 4},	{1, 5, 1, 5},	{1, 10, 5, 6},
	{1, 12, 1, 1},	{1, 12, 1, 2},	{1, 13, 1, 1},	{1, 13, 1, 3},
	{1, 14, 0, 0},	{1, 16, 1, 4},	{1, 16, 1, 6},	{1, 16, 1, 8},
	{1, 16, 1, 10},	{1, 16, 2, 8},	{1, 17, 8, 12},	{1, 18, 0, 0},
/* 80 */{2, 1, 3, 4},	{2, 1, 4, 6},	{2, 2, 1, 4},	{2, 2, 2, 4},
	{2, 2, 4, 4},	{2, 4, 1, 4},	{2, 4, 1, 7},	{2, 5, 1, 5},
	{2, 7, 1, 6},	{3, 1, 1, 4},	{3, 5, 1, 8},	{3, 13, 1, 4},
	{3, 7, 0, 0},	{4, 1, 1, 1},	{4, 1, 1, 4},	{4, 2, 1, 2},
	{4, 2, 1, 6},	{4, 5, 0, 0},	{4, 7, 0, 0},	{4, 10, 0, 0},
/*100 */{4, 13, 1, 6},	{5, 1, 2, 6},	{5, 1, 3, 7},	{5, 1, 4, 6},
	{5, 1, 10, 12},	{5, 2, 1, 3},	{5, 2, 3, 6},	{5, 2, 3, 12},
	{5, 5, 4, 4},	{5, 9, 3, 7},	{5, 9, 4, 5},	{5, 12, 1, 6},
	{6, 2, 1, 3},	{6, 2, 2, 8},	{6, 2, 4, 4},	{6, 5, 1, 10},
	{6, 5, 2, 3},	{6, 8, 1, 5},	{6, 9, 2, 6},	{6, 9, 3, 6},
/*120 */{7, 1, 3, 6},	{7, 2, 1, 3},	{7, 2, 1, 6},	{7, 2, 3, 6},
	{7, 2, 3, 10},	{7, 5, 1, 6},	{7, 5, 2, 3},	{7, 5, 2, 6},
	{7, 5, 4, 4},	{7, 12, 1, 4},	{8, 1, 3, 8},	{8, 2, 1, 3},
	{8, 2, 2, 6},	{8, 2, 3, 8},	{8, 2, 5, 5},	{8, 5, 5, 4},
	{9, 5, 1, 2},	{9, 5, 2, 5},	{9, 5, 2, 6},	{9, 8, 2, 4},
/*140 */{9, 12, 1, 3},	{10, 2, 1, 6},	{10, 4, 1, 1},	{10, 7, 2, 6},
	{10, 9, 1, 2},	{11, 1, 1, 2},	{11, 7, 0, 0},	{11, 13, 2, 4},
	{12, 5, 0, 0},	{13, 5, 0, 0},	{13, 19, 0, 0},	{14, 1, 1, 3},
	{14, 1, 3, 4},	{14, 2, 1, 3},	{14, 2, 1, 4},	{14, 2, 1, 5},
	{14, 2, 1, 6},	{14, 2, 1, 10},	{14, 2, 2, 4},	{14, 2, 2, 5},
/*160 */{14, 2, 2, 6},	{14, 2, 3, 4},	{14, 2, 3, 9},	{14, 2, 4, 4},
	{14, 4, 1, 2},	{14, 4, 1, 4},	{14, 4, 1, 8},	{14, 4, 2, 5},
	{14, 5, 1, 2},	{14, 5, 1, 3},	{14, 5, 2, 4},	{14, 5, 2, 6},
	{14, 5, 3, 5},	{14, 12, 1, 2},	{14, 12, 1, 4},	{14, 13, 2, 4},
	{15, 2, 1, 6},	{15, 2, 3, 6},	{15, 5, 1, 8},	{15, 5, 2, 8},
/*180 */{15, 5, 2, 10},	{15, 5, 2, 12},	{15, 12, 1, 3},	{16, 13, 1, 2},
	{17, 3, 1, 10},	{18, 5, 0, 0},	{19, 5, 5, 8},	{19, 5, 12, 8},
	{19, 5, 14, 8},	{19, 5, 15, 8},	{19, 5, 18, 8},	{19, 5, 20, 8},
	{19, 5, 22, 8},	{19, 5, 26, 8},	{19, 5, 30, 8},	{19, 5, 32, 8},
	{19, 5, 34, 8},	{19, 5, 36, 8},	{19, 5, 38, 8},	{19, 5, 42, 8},
/*200 */{19, 5, 44, 8},	{19, 5, 46, 8},	{19, 5, 52, 8},	{20, 10, 0, 0},
	{21, 1, 0, 0},	{21, 5, 0, 0},	{21, 5, 1, 6},	{21, 7, 0, 0},
	{21, 12, 1, 4},	{22, 5, 2, 3},	{22, 12, 0, 0},	{22, 15, 1, 1},
/*212 */{23, 1, 1, 1},	{23, 5, 1, 3},	{24, 5, 0, 0}
};


monster_type m_list[MAX_MALLOC];
int16 m_level[MAX_MONS_LEVEL+1];

/* Blank monster values	*/
monster_type blank_monster = {0,0,0,0,0,0,0,FALSE,0,FALSE};
int16 mfptr;			/* Cur free monster ptr	*/
int16 mon_tot_mult;		/* # of repro's of creature	*/


