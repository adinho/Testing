/*-------------------------------------------------------------------------*
 * GNU Prolog                                                              *
 *                                                                         *
 * Part  : Prolog buit-in predicates                                       *
 * File  : term_inl_c.c                                                    *
 * Descr.: term (inline) management - C part                               *
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

/* $Id: term_inl_c.c,v 1.15 2009/01/23 11:24:14 diaz Exp $ */

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

	  /* Term comparison inlines */




/*-------------------------------------------------------------------------*
 * PL_BLT_TERM_EQ                                                          *
 *                                                                         *
 *-------------------------------------------------------------------------*/
Bool FC
Pl_Blt_Term_Eq(WamWord x, WamWord y)
{
  return Pl_Term_Compare(x, y) == 0;
}




/*-------------------------------------------------------------------------*
 * PL_BLT_TERM_NEQ                                                         *
 *                                                                         *
 *-------------------------------------------------------------------------*/
Bool FC
Pl_Blt_Term_Neq(WamWord x, WamWord y)
{
  return Pl_Term_Compare(x, y) != 0;
}




/*-------------------------------------------------------------------------*
 * PL_BLT_TERM_LT                                                          *
 *                                                                         *
 *-------------------------------------------------------------------------*/
Bool FC
Pl_Blt_Term_Lt(WamWord x, WamWord y)
{
  return Pl_Term_Compare(x, y) < 0;
}




/*-------------------------------------------------------------------------*
 * PL_BLT_TERM_LTE                                                         *
 *                                                                         *
 *-------------------------------------------------------------------------*/
Bool FC
Pl_Blt_Term_Lte(WamWord x, WamWord y)
{
  return Pl_Term_Compare(x, y) <= 0;
}




/*-------------------------------------------------------------------------*
 * PL_BLT_TERM_GT                                                          *
 *                                                                         *
 *-------------------------------------------------------------------------*/
Bool FC
Pl_Blt_Term_Gt(WamWord x, WamWord y)
{
  return Pl_Term_Compare(x, y) > 0;
}




/*-------------------------------------------------------------------------*
 * PL_BLT_TERM_GTE                                                         *
 *                                                                         *
 *-------------------------------------------------------------------------*/
Bool FC
Pl_Blt_Term_Gte(WamWord x, WamWord y)
{
  return Pl_Term_Compare(x, y) >= 0;
}




/*-------------------------------------------------------------------------*
 * PL_BLT_COMPARE                                                          *
 *                                                                         *
 *-------------------------------------------------------------------------*/
Bool FC
Pl_Blt_Compare(WamWord cmp_word, WamWord x, WamWord y)
{
  int cmp;
  char c;
  Bool res;

  Pl_Set_C_Bip_Name("compare", 3);

  cmp = Pl_Term_Compare(x, y);
  c = (cmp < 0) ? '<' : (cmp == 0) ? '=' : '>';

  res = Pl_Un_Atom_Check(ATOM_CHAR(c), cmp_word);

  Pl_Unset_C_Bip_Name();

  return res;
}




/*-------------------------------------------------------------------------*
 * PL_BLT_ARG                                                              *
 *                                                                         *
 *-------------------------------------------------------------------------*/
Bool FC
Pl_Blt_Arg(WamWord arg_no_word, WamWord term_word, WamWord sub_term_word)
{
  WamWord *arg_adr;
  int func, arity;
  int arg_no;

  Pl_Set_C_Bip_Name("arg", 3);

  arg_no = Pl_Rd_Positive_Check(arg_no_word) - 1;
  arg_adr = Pl_Rd_Compound_Check(term_word, &func, &arity);

  Pl_Unset_C_Bip_Name();

  return (unsigned) arg_no < (unsigned) arity &&
    Pl_Unify(sub_term_word, arg_adr[arg_no]);
}




/*-------------------------------------------------------------------------*
 * PL_BLT_FUNCTOR                                                          *
 *                                                                         *
 *-------------------------------------------------------------------------*/
Bool FC
Pl_Blt_Functor(WamWord term_word, WamWord functor_word, WamWord arity_word)
{
  WamWord word, tag_mask;
  WamWord *adr;
  WamWord tag_functor;
  int arity;
  Bool res;


  Pl_Set_C_Bip_Name("functor", 3);

  DEREF(term_word, word, tag_mask);
  if (tag_mask != TAG_REF_MASK)
    {
      if (tag_mask == TAG_LST_MASK)
	res = Pl_Un_Atom_Check(ATOM_CHAR('.'), functor_word) &&
	  Pl_Un_Integer_Check(2, arity_word);
      else if (tag_mask == TAG_STC_MASK)
	{
	  adr = UnTag_STC(word);
	  res = Pl_Un_Atom_Check(Functor(adr), functor_word) &&
	    Pl_Un_Integer_Check(Arity(adr), arity_word);
	}
      else
	res = Pl_Unify(word, functor_word) && Pl_Un_Integer_Check(0, arity_word);

      goto finish;
    }


				/* tag_mask == TAG_REF_MASK */

  DEREF(functor_word, word, tag_mask);
  if (tag_mask == TAG_REF_MASK)
    Pl_Err_Instantiation();

  if (tag_mask != TAG_ATM_MASK && tag_mask != TAG_INT_MASK && 
      tag_mask != TAG_FLT_MASK)
    Pl_Err_Type(pl_type_atomic, functor_word);

  tag_functor = tag_mask;
  functor_word = word;

  arity = Pl_Rd_Positive_Check(arity_word);

  if (arity > MAX_ARITY)
    Pl_Err_Representation(pl_representation_max_arity);

  if (tag_functor == TAG_ATM_MASK && UnTag_ATM(functor_word) == ATOM_CHAR('.')
      && arity == 2)
    {
      res = (Pl_Get_List(term_word)) ? Pl_Unify_Void(2), TRUE : FALSE;
      goto finish;
    }

  if (tag_functor == TAG_ATM_MASK && arity > 0)
    {
      res = (Pl_Get_Structure(UnTag_ATM(functor_word), arity, term_word)) ?
	Pl_Unify_Void(arity), TRUE : FALSE;
      goto finish;
    }

  if (arity != 0)
    Pl_Err_Type(pl_type_atom, functor_word);

  res = Pl_Unify(functor_word, term_word);

finish:
  Pl_Unset_C_Bip_Name();

  return res;
}




/*-------------------------------------------------------------------------*
 * PL_BLT_UNIV                                                             *
 *                                                                         *
 *-------------------------------------------------------------------------*/
Bool FC
Pl_Blt_Univ(WamWord term_word, WamWord list_word)
{
  WamWord word, tag_mask;
  WamWord *adr;
  WamWord car_word;
  int lst_length;
  WamWord *arg1_adr;
  WamWord *term_adr, *lst_adr, *stc_adr;
  WamWord functor_word, functor_tag;
  int functor;
  int arity;


  Pl_Set_C_Bip_Name("=..", 2);

  DEREF(term_word, word, tag_mask);

  if (tag_mask == TAG_REF_MASK)
    goto list_to_term;

				/* from term to list functor+args */

  if (tag_mask == TAG_LST_MASK)
    {
      adr = UnTag_LST(word);
      car_word = Tag_ATM(ATOM_CHAR('.'));
      lst_length = 1 + 2;
      arg1_adr = &Car(adr);
    }
  else if (tag_mask == TAG_STC_MASK)
    {
      adr = UnTag_STC(word);
      car_word = Tag_ATM(Functor(adr));
      lst_length = 1 + Arity(adr);
      arg1_adr = &Arg(adr, 0);
    }
#ifndef NO_USE_FD_SOLVER
  else if (tag_mask == TAG_FDV_MASK)
    {
      adr = UnTag_FDV(word);
      car_word = Tag_REF(adr);	/* since Dont_Separate_Tag */
      lst_length = 1 + 0;
    } 
#endif
  else				/* TAG_ATM/INT/FLT_MASK */
    {
      car_word = word;
      lst_length = 1 + 0;
    }

  Pl_Check_For_Un_List(list_word);

  Pl_Unset_C_Bip_Name();

  for (;;)
    {
      if (!Pl_Get_List(list_word) || !Pl_Unify_Value(car_word))
	return FALSE;

      list_word = Pl_Unify_Variable();

      if (--lst_length == 0)
	break;

      car_word = *arg1_adr++;
    }

  return Pl_Get_Nil(list_word);

  /* from list functor+args to term */

list_to_term:

  term_adr = UnTag_REF(word);

  DEREF(list_word, word, tag_mask);
  if (tag_mask == TAG_REF_MASK)
    Pl_Err_Instantiation();

  if (word == NIL_WORD)
    Pl_Err_Domain(pl_domain_non_empty_list, list_word);

  if (tag_mask != TAG_LST_MASK)
    Pl_Err_Type(pl_type_list, list_word);

  lst_adr = UnTag_LST(word);
  DEREF(Car(lst_adr), functor_word, functor_tag);
  if (functor_tag == TAG_REF_MASK)
    Pl_Err_Instantiation();

  DEREF(Cdr(lst_adr), word, tag_mask);

  if (word == NIL_WORD)
    {
      if (functor_tag != TAG_ATM_MASK && functor_tag != TAG_INT_MASK &&
	  functor_tag != TAG_FLT_MASK)
	Pl_Err_Type(pl_type_atomic, functor_word);

      term_word = functor_word;
      goto finish;
    }

  if (functor_tag != TAG_ATM_MASK)
    Pl_Err_Type(pl_type_atom, functor_word);

  if (tag_mask == TAG_REF_MASK)
    Pl_Err_Instantiation();

  if (tag_mask != TAG_LST_MASK)
    Pl_Err_Type(pl_type_list, list_word);

  functor = UnTag_ATM(functor_word);

  stc_adr = H;

  H++;				/* space for f/n maybe lost if a list */
  arity = 0;

  for (;;)
    {
      arity++;
      lst_adr = UnTag_LST(word);
      DEREF(Car(lst_adr), word, tag_mask);
      Do_Copy_Of_Word(tag_mask, word); /* since Dont_Separate_Tag */
      Global_Push(word);

      DEREF(Cdr(lst_adr), word, tag_mask);
      if (word == NIL_WORD)
	break;

      if (tag_mask == TAG_REF_MASK)
	Pl_Err_Instantiation();

      if (tag_mask != TAG_LST_MASK)
	Pl_Err_Type(pl_type_list, list_word);
    }

  if (arity > MAX_ARITY)
    Pl_Err_Representation(pl_representation_max_arity);

  if (functor == ATOM_CHAR('.') && arity == 2)	/* a list */
    term_word = Tag_LST(stc_adr + 1);
  else
    {
      *stc_adr = Functor_Arity(functor, arity);
      term_word = Tag_STC(stc_adr);
    }

finish:
  Bind_UV(term_adr, term_word);
  Pl_Unset_C_Bip_Name();
  return TRUE;
}




/*-------------------------------------------------------------------------*
 * PL_COPY_TERM_2                                                          *
 *                                                                         *
 *-------------------------------------------------------------------------*/
Bool
Pl_Copy_Term_2(WamWord u_word, WamWord v_word)
{
  WamWord word;
  int size;
/* fix_bug is because when gcc sees &xxx where xxx is a fct argument variable
 * it allocates a frame even with -fomit-frame-pointer.
 * This corrupts ebp on ix86 */
  static WamWord fix_bug;

  size = Pl_Term_Size(u_word);
  fix_bug = u_word;	
  Pl_Copy_Term(H, &fix_bug);
  word = *H;
  H += size;

  return Pl_Unify(word, v_word);
}




/*-------------------------------------------------------------------------*
 * PL_SETARG_4                                                             *
 *                                                                         *
 *-------------------------------------------------------------------------*/
Bool
Pl_Setarg_4(WamWord arg_no_word, WamWord term_word, WamWord new_value_word,
	 WamWord undo_word)
{
  WamWord word, tag_mask;
  int func, arity;
  int undo;
  WamWord *arg_adr;
  int arg_no;

  arg_adr = Pl_Rd_Compound_Check(term_word, &func, &arity);
  arg_no = Pl_Rd_Positive_Check(arg_no_word) - 1;
  undo = Pl_Rd_Boolean_Check(undo_word);

  DEREF(new_value_word, word, tag_mask);
  if (!undo && tag_mask != TAG_ATM_MASK && tag_mask != TAG_INT_MASK)
    Pl_Err_Type(pl_type_atomic, word);	/* pl_type_atomic but float not allowed */

  if ((unsigned) arg_no >= (unsigned) arity)
    return FALSE;

  if (undo)
    Bind_OV((arg_adr + arg_no), word);
  else
    arg_adr[arg_no] = word;

  return TRUE;
}




/*-------------------------------------------------------------------------*
 * PL_TERM_REF_2                                                           *
 *                                                                         *
 *-------------------------------------------------------------------------*/
Bool
Pl_Term_Ref_2(WamWord term_word, WamWord ref_word)
{
  WamWord word, tag_mask;
  WamWord word1, *adr;
  int ref;
				/* my own DEREF here to get the address */
  adr = NULL;			/* added this */
  word = term_word;
  do
    {
      word1 = word;
      tag_mask = Tag_Mask_Of(word);
      if (tag_mask != TAG_REF_MASK)
	break;

      adr = UnTag_REF(word);	/* added this */
      word = *adr;
    }
  while (word != word1);

  if (tag_mask == TAG_REF_MASK)
    {
      ref = Pl_Rd_Positive_Check(ref_word);
      adr = Global_Stack + ref;
      return Pl_Unify(word, *adr);
    }

  if (adr < Global_Stack || adr > H)
    {
      adr = H;
      Global_Push(word);
    }
  ref = Global_Offset(adr);

  return Pl_Un_Positive_Check(ref, ref_word);
}
