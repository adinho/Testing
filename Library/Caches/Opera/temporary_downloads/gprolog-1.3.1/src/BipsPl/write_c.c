/*-------------------------------------------------------------------------*
 * GNU Prolog                                                              *
 *                                                                         *
 * Part  : Prolog buit-in predicates                                       *
 * File  : write_c.c                                                       *
 * Descr.: term output (write/1 and friends) management - C part           *
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

/* $Id: write_c.c,v 1.14 2009/01/23 11:24:14 diaz Exp $ */

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




/*-------------------------------------------------------------------------*
 * PL_WRITE_TERM_2                                                         *
 *                                                                         *
 *-------------------------------------------------------------------------*/
void
Pl_Write_Term_2(WamWord sora_word, WamWord term_word)
{
  int stm;
  StmInf *pstm;


  stm = (sora_word == NOT_A_WAM_WORD)
    ? pl_stm_output : Pl_Get_Stream_Or_Alias(sora_word, STREAM_CHECK_OUTPUT);
  pstm = pl_stm_tbl[stm];

  pl_last_output_sora = sora_word;
  Pl_Check_Stream_Type(stm, TRUE, FALSE);

  Pl_Write_Term(pstm, SYS_VAR_WRITE_DEPTH, SYS_VAR_WRITE_PREC, 
	     SYS_VAR_OPTION_MASK, term_word);
}




/*-------------------------------------------------------------------------*
 * PL_WRITE_TERM_1                                                         *
 *                                                                         *
 *-------------------------------------------------------------------------*/
void
Pl_Write_Term_1(WamWord term_word)
{
  Pl_Write_Term_2(NOT_A_WAM_WORD, term_word);
}




/*-------------------------------------------------------------------------*
 * PL_WRITE_1                                                              *
 *                                                                         *
 *-------------------------------------------------------------------------*/
void
Pl_Write_1(WamWord term_word)
{
  SYS_VAR_OPTION_MASK = WRITE_NUMBER_VARS | WRITE_NAME_VARS;
  SYS_VAR_WRITE_DEPTH = -1;
  SYS_VAR_WRITE_PREC = MAX_PREC;

  Pl_Write_Term_2(NOT_A_WAM_WORD, term_word);
}




/*-------------------------------------------------------------------------*
 * PL_WRITE_2                                                              *
 *                                                                         *
 *-------------------------------------------------------------------------*/
void
Pl_Write_2(WamWord sora_word, WamWord term_word)
{
  SYS_VAR_OPTION_MASK = WRITE_NUMBER_VARS | WRITE_NAME_VARS;
  SYS_VAR_WRITE_DEPTH = -1;
  SYS_VAR_WRITE_PREC = MAX_PREC;

  Pl_Write_Term_2(sora_word, term_word);
}




/*-------------------------------------------------------------------------*
 * PL_WRITEQ_1                                                             *
 *                                                                         *
 *-------------------------------------------------------------------------*/
void
Pl_Writeq_1(WamWord term_word)
{
  SYS_VAR_OPTION_MASK = WRITE_NUMBER_VARS | WRITE_NAME_VARS | WRITE_QUOTED;
  SYS_VAR_WRITE_DEPTH = -1;
  SYS_VAR_WRITE_PREC = MAX_PREC;

  Pl_Write_Term_2(NOT_A_WAM_WORD, term_word);
}




/*-------------------------------------------------------------------------*
 * PL_WRITEQ_2                                                             *
 *                                                                         *
 *-------------------------------------------------------------------------*/
void
Pl_Writeq_2(WamWord sora_word, WamWord term_word)
{
  SYS_VAR_OPTION_MASK = WRITE_NUMBER_VARS | WRITE_NAME_VARS | WRITE_QUOTED;
  SYS_VAR_WRITE_DEPTH = -1;
  SYS_VAR_WRITE_PREC = MAX_PREC;

  Pl_Write_Term_2(sora_word, term_word);
}




/*-------------------------------------------------------------------------*
 * PL_WRITE_CANONICAL_1                                                    *
 *                                                                         *
 *-------------------------------------------------------------------------*/
void
Pl_Write_Canonical_1(WamWord term_word)
{
  SYS_VAR_OPTION_MASK = WRITE_IGNORE_OP | WRITE_QUOTED;
  SYS_VAR_WRITE_DEPTH = -1;
  SYS_VAR_WRITE_PREC = MAX_PREC;

  Pl_Write_Term_2(NOT_A_WAM_WORD, term_word);
}




/*-------------------------------------------------------------------------*
 * PL_WRITE_CANONICAL_2                                                    *
 *                                                                         *
 *-------------------------------------------------------------------------*/
void
Pl_Write_Canonical_2(WamWord sora_word, WamWord term_word)
{
  SYS_VAR_OPTION_MASK = WRITE_IGNORE_OP | WRITE_QUOTED;
  SYS_VAR_WRITE_DEPTH = -1;
  SYS_VAR_WRITE_PREC = MAX_PREC;

  Pl_Write_Term_2(sora_word, term_word);
}




/*-------------------------------------------------------------------------*
 * PL_DISPLAY_1                                                            *
 *                                                                         *
 *-------------------------------------------------------------------------*/
void
Pl_Display_1(WamWord term_word)
{
  SYS_VAR_OPTION_MASK = WRITE_IGNORE_OP;
  SYS_VAR_WRITE_DEPTH = -1;
  SYS_VAR_WRITE_PREC = MAX_PREC;

  Pl_Write_Term_2(NOT_A_WAM_WORD, term_word);
}




/*-------------------------------------------------------------------------*
 * PL_DISPLAY_2                                                            *
 *                                                                         *
 *-------------------------------------------------------------------------*/
void
Pl_Display_2(WamWord sora_word, WamWord term_word)
{
  SYS_VAR_OPTION_MASK = WRITE_IGNORE_OP;
  SYS_VAR_WRITE_DEPTH = -1;
  SYS_VAR_WRITE_PREC = MAX_PREC;

  Pl_Write_Term_2(sora_word, term_word);
}




/*-------------------------------------------------------------------------*
 * PL_PRINT_1                                                              *
 *                                                                         *
 * NB: the definition of the predicate print/1-2 is in the file print.pl   *
 * to avoid to link call/1 if print is not used.                           *
 *-------------------------------------------------------------------------*/
void
Pl_Print_1(WamWord term_word)
{
  SYS_VAR_OPTION_MASK =
    WRITE_NUMBER_VARS | WRITE_NAME_VARS | WRITE_PORTRAYED;
  SYS_VAR_WRITE_DEPTH = -1;
  SYS_VAR_WRITE_PREC = MAX_PREC;

  Pl_Write_Term_2(NOT_A_WAM_WORD, term_word);
}




/*-------------------------------------------------------------------------*
 * PL_PRINT_2                                                              *
 *                                                                         *
 *-------------------------------------------------------------------------*/
void
Pl_Print_2(WamWord sora_word, WamWord term_word)
{
  SYS_VAR_OPTION_MASK =
    WRITE_NUMBER_VARS | WRITE_NAME_VARS | WRITE_PORTRAYED;
  SYS_VAR_WRITE_DEPTH = -1;
  SYS_VAR_WRITE_PREC = MAX_PREC;

  Pl_Write_Term_2(sora_word, term_word);
}




/*-------------------------------------------------------------------------*
 * PL_NL_1                                                                 *
 *                                                                         *
 *-------------------------------------------------------------------------*/
void
Pl_Nl_1(WamWord sora_word)
{
  int stm;


  stm = (sora_word == NOT_A_WAM_WORD)
    ? pl_stm_output : Pl_Get_Stream_Or_Alias(sora_word, STREAM_CHECK_OUTPUT);

  pl_last_output_sora = sora_word;
  Pl_Check_Stream_Type(stm, TRUE, FALSE);

  Pl_Stream_Putc('\n', pl_stm_tbl[stm]);
}




/*-------------------------------------------------------------------------*
 * PL_NL_0                                                                 *
 *                                                                         *
 *-------------------------------------------------------------------------*/
void
Pl_Nl_0(void)
{
  Pl_Nl_1(NOT_A_WAM_WORD);
}
