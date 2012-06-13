/*-------------------------------------------------------------------------*
 * GNU Prolog                                                              *
 *                                                                         *
 * Part  : Prolog buit-in predicates                                       *
 * File  : control_c.c                                                     *
 * Descr.: control management - C part                                     *
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

/* $Id: control_c.c,v 1.15 2009/01/23 11:24:13 diaz Exp $ */

#include "engine_pl.h"
#include "bips_pl.h"




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

#define FOR_ALT                    X24666F725F616C74

Prolog_Prototype(FOR_ALT, 0);




/*-------------------------------------------------------------------------*
 * PL_HALT_IF_NO_TOP_LEVEL_1                                               *
 *                                                                         *
 *-------------------------------------------------------------------------*/
WamCont
Pl_Halt_If_No_Top_Level_1(WamWord exit_code_word)
{
  PredInf *pred;
  int x;

  x = Pl_Rd_Integer_Check(exit_code_word);

  if (SYS_VAR_TOP_LEVEL == 0)	/* no top level running */
    Pl_Exit_With_Value(x);

  pred =
    Pl_Lookup_Pred(Pl_Create_Atom((x) ? "$top_level_abort" : "$top_level_stop"),
		0);

  if (pred == NULL)		/* should not occur */
    Pl_Exit_With_Value(x);

  return (WamCont) (pred->codep);
}




/*-------------------------------------------------------------------------*
 * PL_HALT_1                                                               *
 *                                                                         *
 *-------------------------------------------------------------------------*/
void
Pl_Halt_1(WamWord exit_code_word)
{
  Pl_Exit_With_Value(Pl_Rd_Integer_Check(exit_code_word));
}




/*-------------------------------------------------------------------------*
 * PL_FOR_3                                                                *
 *                                                                         *
 *-------------------------------------------------------------------------*/
Bool
Pl_For_3(WamWord i_word, WamWord l_word, WamWord u_word)
{
  WamWord word, tag_mask;
  int i, l, u;

  l = Pl_Rd_Integer_Check(l_word);
  u = Pl_Rd_Integer_Check(u_word);

  DEREF(i_word, word, tag_mask);
  if (tag_mask != TAG_REF_MASK)
    {
      i = Pl_Rd_Integer_Check(word);
      return i >= l && i <= u;
    }
  i_word = word;

  if (l > u)
    return FALSE;
				/* here i_word is a variable */
  if (l < u)			/* non deterministic case */
    {
      A(0) = i_word;
      A(1) = l + 1;
      A(2) = u;
      Pl_Create_Choice_Point((CodePtr) Prolog_Predicate(FOR_ALT, 0), 3);
    }

  return Pl_Get_Integer(l, i_word); /* always TRUE */
}




/*-------------------------------------------------------------------------*
 * PL_FOR_ALT_0                                                            *
 *                                                                         *
 *-------------------------------------------------------------------------*/
void
Pl_For_Alt_0(void)
{
  WamWord i_word;
  int l, u;

  Pl_Update_Choice_Point((CodePtr) Prolog_Predicate(FOR_ALT, 0), 0);

  i_word = AB(B, 0);
  l = AB(B, 1);
  u = AB(B, 2);

  /* here i_word is a variable */
  if (l == u)
    Delete_Last_Choice_Point();
  else				/* non deterministic case */
    {
#if 0 /* the following data is unchanged */
      AB(B,0)=i_word;
#endif
      AB(B, 1) = l + 1;
#if 0 /* the following data is unchanged */
      AB(B,2)=u;
#endif
    }

  Pl_Get_Integer(l, i_word);	/* always TRUE */
}
