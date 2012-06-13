/*-------------------------------------------------------------------------*
 * GNU Prolog                                                              *
 *                                                                         *
 * Part  : Prolog engine                                                   *
 * File  : main.c                                                          *
 * Descr.: main                                                            *
 * Author: Daniel Diaz                                                     *
 *                                                                         *
 * Copyright (C) 1999-2009 Daniel Diaz                                     *
 *                                                                         *
 * GNU Prolog is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation; either version 2, or any later version.       *
 *                                                                         *
 * GNU Prolog is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU        *
 * General Public License for more details.                                *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc.  *
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA.               *
 *-------------------------------------------------------------------------*/

/* $Id: main.c,v 1.17 2009/01/23 11:24:14 diaz Exp $ */

#include <stdio.h>
#include <string.h>

#include "engine_pl.h"


/*---------------------------------*
 * Constants                       *
 *---------------------------------*/

/*---------------------------------*
 * Type Definitions                *
 *---------------------------------*/

/*---------------------------------*
 * Global Variables                *
 *---------------------------------*/

/*---------------------------------*
 * Function Prototypes             *
 *---------------------------------*/




/*-------------------------------------------------------------------------*
 * MAIN                                                                    *
 *                                                                         *
 * A problem appeared in GCC 3.0.x under Linux/ix86:                       *
 * the main() function always use a frame (and thus ebp). This causes a bug*
 * if ebp is used by gcc between Pl_Start_Prolog() and Pl_Stop_Prolog()    *
 * (e.g. to access argv/argc or local variables) since ebp contains a WAM  *
 * register. Note that after Pl_Stop_Prolog() all registers are restored   *
 * and ebp is correct when returning in main().                            *
 *                                                                         *
 * To solve we can use an intermediate function Main_Wrapper() called by   *
 * the main() function.                                                    *
 *                                                                         *
 * Another solution consists in passing -mpreferred-stack-boundary=2 to gcc*
 * since it gcc uses ebp to ensure the stack alignment (to 4).             *
 *                                                                         *
 * This main function uses the wrapper even if ebp is not really used      *
 * between Pl_Start_Prolog() and Pl_Stop_Prolog() but to serve as model.   *
 *-------------------------------------------------------------------------*/

static int
Main_Wrapper(int argc, char *argv[])
{
  int nb_user_directive;
  Bool top_level;

  nb_user_directive = Pl_Start_Prolog(argc, argv);

  top_level = Pl_Try_Execute_Top_Level();

  Pl_Stop_Prolog();

  if (top_level || nb_user_directive)
    return 0;

  fprintf(stderr,
          "Warning: no initial goal executed\n"
          "   use a directive :- initialization(Goal)\n"
          "   or remove the link option --no-top-level"
          " (or --min-bips or --min-size)\n");

  return 1;
}


int
main(int argc, char *argv[])
{
  Pl_Exit_With_Value(Main_Wrapper(argc, argv));
  return 0;			/* anything for the compiler */
}
