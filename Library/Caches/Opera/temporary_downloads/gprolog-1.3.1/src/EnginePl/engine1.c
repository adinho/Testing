/*-------------------------------------------------------------------------*
 * GNU Prolog                                                              *
 *                                                                         *
 * Part  : Prolog engine                                                   *
 * File  : engine1.c                                                       *
 * Descr.: general engine                                                  *
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

/* $Id: engine1.c,v 1.22 2009/01/23 11:24:14 diaz Exp $ */

#include "engine_pl.h"


WamWord *pl_ensure_reserved;


/*-------------------------------------------------------------------------*
 * Call_Compiled invokes a Prolog code.                                    *
 * reserved_stack_space is only used to reserve some space in the stack and*
 * should not be removed by the compiler. This allows prolog compiled code *
 * to use this space to pass the arguments to C run-time functions, without*
 * having to decrement $sp (ie. push/pop) (e.g. on ix86).                  *
 * Note finally that since this function is called inside Call_Next, we are*
 * sure that $sp at longjmp will be more recent (ie. <) than $sp at setjmp.*
 * Certain implementations of setjmp/longjmp requires this.                *
 *                                                                         *
 * Warning: do not use nested { ... } blocs to declare register variables  *
 * The C compiler "forgets" them after the bloc (e.g. solaris 2.6).        *
 *                                                                         *
 * This functions is in a separate file for historical reasons. In versions*
 * <= 1.2.4, this file was compiled without any C compiler optimization to *
 * ensure reserved_stack_space was not removed but the C compiler. In order*
 * to use ebp under ix86 it must be compiled with -fomit-frame-pointer. The*
 * simpliest way was to use the same C compiler invocation but adding a    *
 * global variable to ensure the stack is not removed.                     *
 *-------------------------------------------------------------------------*/

void
Pl_Call_Compiled(CodePtr codep)
{
  WamWord reserved_stack_space[1024];

#if defined(M_sparc) && !defined(M_sparc_bsd)
  register long * __attribute__ ((unused)) rfl asm("%l2") = pl_base_fl;
  register double * __attribute__ ((unused)) rfd asm("%l3") = pl_base_fd;
  pl_ensure_reserved = (WamWord *) rfl + (long) rfd; /* to avoid gcc remove 2 previous inits ! */
#endif

#if !defined(NO_MACHINE_REG_FOR_REG_BANK) && !defined(MAP_REG_BANK)

#if defined(M_ix86_linux) || \
    defined(M_ix86_cygwin) || defined(M_ix86_mingw) || \
    defined(M_ix86_sco) || defined(M_ix86_bsd) 

  register WamWord *rb asm("%ebx") = pl_reg_bank;
  pl_ensure_reserved = (WamWord *) rb; /* to avoid gcc warning */

#elif defined(_MSC_VER)

  _asm mov ebx, pl_reg_bank

#elif defined(M_mips_irix)

  register WamWord *rb asm("$16") = pl_reg_bank;
  pl_ensure_reserved = (WamWord *) rb; /* to avoid gcc warning */

#elif defined(M_alpha_linux) || defined(M_alpha_osf)

  register WamWord *rb asm("$9") = pl_reg_bank;
  pl_ensure_reserved = (WamWord *) rb; /* to avoid gcc warning */

#elif defined(M_powerpc_linux)

  register WamWord *rb asm("15") = pl_reg_bank;
  pl_ensure_reserved = (WamWord *) rb; /* to avoid gcc warning */

#elif defined(M_powerpc_darwin)

  register WamWord *rb asm("r15") = pl_reg_bank;
  pl_ensure_reserved = (WamWord *) rb; /* to avoid gcc warning */

#elif defined(M_sparc)

  register WamWord *rb asm("%l0") = pl_reg_bank;
  pl_ensure_reserved = (WamWord *) rb; /* to avoid gcc warning */

#elif defined(M_x86_64)

  register WamWord *rb asm("%r12") = pl_reg_bank;
  pl_ensure_reserved = (WamWord *) rb; /* to avoid gcc warning */

#endif

#endif

  pl_ensure_reserved = reserved_stack_space;
#if defined(M_ix86_darwin)	/* see comment in Ma2Asm/ix86_any.c */
  asm("andl $0xfffffff0,%esp");
  asm("addl $4,%esp");
#elif defined(M_x86_64)		/* see comment in Ma2Asm/x86_64_any.c */
  asm("andq $0xfffffffffffffff0,%rsp");
  asm("addq $8,%rsp");
#endif

  (*codep) ();

}
