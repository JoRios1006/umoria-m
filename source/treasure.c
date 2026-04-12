/* source/treasure.c: dungeon object definitions

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

/* Following are treasure arrays        and variables                   */

/* Object description:  Objects are defined here.  Each object has
  the following attributes:
        Descriptor : Name of item and formats.
                & is replaced with 'a', 'an', or a number.
                ~ is replaced with null or 's'.
        Character  : Character that represents the item.
        Type value : Value representing the type of object.
        Sub value  : separate value for each item of a type.
                0 - 63: object can not stack
                64 - 127: dungeon object, can stack with other D object
                128 - 191: unused, previously for store items
                192: stack with other iff have same p1 value, always
                        treated as individual objects
                193 - 255: object can stack with others iff they have
                        the same p1 value, usually considered one group
                Objects which have two type values, e.g. potions and
                scrolls, need to have distinct subvals for
                each item regardless of its tval
        Damage     : amount of damage item can cause.
        Weight     : relative weight of an item.
        Number     : number of items appearing in group.
        To hit     : magical plusses to hit.
        To damage  : magical plusses to damage.
        AC         : objects relative armor class.
                1 is worse than 5 is worse than 10 etc.
        To AC      : Magical bonuses to AC.
        P1         : Catch all for magical abilities such as
                     plusses to strength, minuses to searching.
        Flags      : Abilities of object.  Each ability is a
                     bit.  Bits 1-31 are used. (Signed integer)
        Level      : Minimum level on which item can be found.
        Cost       : Relative cost of item.

        Special Abilities can be added to item by magic_init(),
        found in misc.c.

        Scrolls, Potions, and Food:
        Flags is used to define a function which reading/quaffing
        will cause.  Most scrolls and potions have only one bit
        set.  Potions will generally have some food value, found
        in p1.

        Wands and Staffs:
        Flags defines a function, p1 contains number of charges
        for item.  p1 is set in magic_init() in misc.c.

        Chests:
        Traps are added randomly by magic_init() in misc.c.     */

/* Object list (All objects must be defined here)                */

char *special_names[SN_ARRAY_SIZE] = {
        CNIL,                   "(R)",                  "(RA)",
        "(RF)",                 "(RC)",                 "(RL)",
        "(HA)",                 "(DF)",                 "(SA)",
        "(SD)",                 "(SE)",                 "(SU)",
        "(FT)",                 "(FB)",                 "of Free Action",
        "of Slaying",           "of Clumsiness",        "of Weakness",
        "of Slow Descent",      "of Speed",             "of Stealth",
        "of Slowness",          "of Noise",             "of Great Mass",
        "of Intelligence",      "of Wisdom",            "of Infra-Vision",
        "of Might",             "of Lordliness",        "of the Magi",
        "of Beauty",            "of Seeing",            "of Regeneration",
        "of Stupidity",         "of Dullness",          "of Blindness",
        "of Timidness",         "of Teleportation",     "of Ugliness",
        "of Protection",        "of Irritation",        "of Vulnerability",
        "of Enveloping",        "of Fire",              "of Slay Evil",
        "of Dragon Slaying",    "(Empty)",              "(Locked)",
        "(Poison Needle)",      "(Gas Trap)",           "(Explosion Device)",
        "(Summoning Runes)",    "(Multiple Traps)",     "(Disarmed)",
        "(Unlocked)",           "of Slay Animal"
};

/* Pairing things down for THINK C.  */
#ifndef RSRC_PART2
#endif

/* Treasure related values                                      */
int16 inven_ctr = 0;            /* Total different obj's        */
int16 inven_weight = 0;         /* Cur carried weight   */
int16 equip_ctr = 0;            /* Cur equipment ctr    */
int16 tcptr;                    /* Cur treasure heap ptr        */


