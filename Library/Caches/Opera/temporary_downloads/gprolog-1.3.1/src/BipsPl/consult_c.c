/*-------------------------------------------------------------------------*
 * GNU Prolog                                                              *
 *                                                                         *
 * Part  : Prolog buit-in predicates                                       *
 * File  : consult_c.c                                                     *
 * Descr.: file consulting - C part                                        *
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

/* $Id: consult_c.c,v 1.18 2009/01/23 11:24:13 diaz Exp $ */

#include <sys/types.h>

#include "engine_pl.h"
#include "bips_pl.h"
#include "linedit.h"

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

#if 1

/*-------------------------------------------------------------------------*
 * PL_CONSULT_2                                                            *
 *                                                                         *
 *-------------------------------------------------------------------------*/
Bool
Pl_Consult_2(WamWord tmp_file_word, WamWord pl_file_word)
{
  char *tmp_file = Pl_Rd_String_Check(tmp_file_word);
  char *pl_file = Pl_Rd_String_Check(pl_file_word);
  StmInf *pstm_o = pl_stm_tbl[pl_stm_top_level_output];
  StmInf *pstm_i = pl_stm_tbl[pl_stm_top_level_input];
  int pid;
  FILE *f_out, *f_in;
  FILE **pf_in;
  long save;
  unsigned char *p = NULL;
  int status, c;
  int save_use_le_prompt;
  char *arg[] = { "pl2wam", "-w", "--compile-msg", "--no-redef-error",
		  "--pl-state", tmp_file, "-o", tmp_file, pl_file,
		  NULL, NULL, NULL, NULL };  /* 3 warnings + 1 for terminal NULL */
  int warn_i = sizeof(arg) / sizeof(arg[0]) - 4; /* the 4 NULL */


#define ADD_WARN(flag, opt_str)  if (!Flag_Value(flag))  arg[warn_i++] = opt_str

  ADD_WARN(FLAG_SUSPICIOUS_WARNING, "--no-susp-warn");
  ADD_WARN(FLAG_SINGLETON_WARNING, "--no-singl-warn");
  ADD_WARN(FLAG_MULTIFILE_WARNING, "--no-mult-warn");


  save = SYS_VAR_SAY_GETC;
#ifndef NO_USE_PIPED_STDIN_FOR_CONSULT
  SYS_VAR_SAY_GETC = 1;
  pf_in = &f_in;
#else
  f_in = NULL;
  pf_in = NULL;
#endif
  Pl_Write_Pl_State_File(tmp_file_word);
  SYS_VAR_SAY_GETC = save;

  Pl_Flush_All_Streams();
  pid = Pl_M_Spawn_Redirect(arg, 0, pf_in, &f_out, &f_out);
  Os_Test_Error(pid == -1);
  if (pid == -2)
    {
    error_pl2wam:
      Pl_Err_System(Pl_Create_Atom("error trying to execute pl2wam "
				"(maybe not found)"));
      return FALSE;
    }

  save_use_le_prompt = pl_use_le_prompt;
  pl_use_le_prompt = 0;
  for (;;)
    {
#if 1
      c = fgetc(f_out);
#else
      char c0;
      c = (read(fileno(f_out), &c0, 1) == 1) ? c0 : EOF;
#endif
      if (c == EOF)
	break;
	  
#ifndef NO_USE_PIPED_STDIN_FOR_CONSULT
      if (c == CHAR_TO_EMIT_WHEN_CHAR)
	{
	  if (p == NULL)
	    {
	      c = Pl_Stream_Getc(pstm_i);
	      if (c == EOF)
		{
		eof_reached:
		  p = (unsigned char *) "end_of_file.\n";
		  c = *p++;
		}
	    }
	  else
	    {
	      if (*p == '\0')
		goto eof_reached;
	      else
		c = *p++;
	    }
	  fputc(c, f_in);
	  fflush(f_in);
	  continue;
	}
#endif
      Pl_Stream_Putc(c, pstm_o);
    }
  pl_use_le_prompt = save_use_le_prompt;

  if (f_in)
    fclose(f_in);
  fclose(f_out);

  status = Pl_M_Get_Status(pid);
  if (status < 0)
    goto error_pl2wam;

  return status == 0;
}

#endif
