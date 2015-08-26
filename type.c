/*
 *
 * type.c
 *
 * Created at:  Tue Aug 11 13:10:40 2015 13:10:40
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "mem.h"
#include "type.h"

static size_t current_id = 0;
static char   current_ch = 'a';

static int curr_var_idd = 16;
static int curr_oper_idd = 256;

type_t *type_variable_(const char *file, unsigned line)
{
  type_t *type = alloc(type_t);

#if DEBUG
  printf("new type variable (%d) in %s line %u\n", curr_var_idd, file, line);
#else
  /* supress warnings */
  (void)file;
  (void)line;
#endif

  type->ttype = TYPE_VARIABLE;
  type->id = current_id++;
  type->ch = 0;
  type->instance = NULL;
  type->idd = curr_var_idd++; /* debug */

  return type;
}

type_t *type_operator_0_(const char *file, unsigned line, ttype_t ttype)
{
  type_t *type = alloc(type_t);

#if DEBUG
  printf("new type operator (%d) in %s line %u\n", curr_oper_idd, file, line);
#else
  /* supress warnings */
  (void)file;
  (void)line;
#endif

  type->ttype = ttype;
  type->types[0] = NULL;
  type->types[1] = NULL;
  type->types_num = 0;
  type->idd = curr_oper_idd++; /* debug */

  return type;
}

type_t *type_operator_1_(const char *file, unsigned line, ttype_t ttype, type_t *type_one)
{
  type_t *type = alloc(type_t);

#if DEBUG
  printf("new type operator (%d) in %s line %u\n", curr_oper_idd, file, line);
#else
  /* supress warnings */
  (void)file;
  (void)line;
#endif

  type->ttype = ttype;
  type->types[0] = type_one;
  type->types[1] = NULL;
  type->types_num = 1;
  type->idd = curr_oper_idd++; /* debug */

  return type;
}

type_t *type_operator_2_(const char *file, unsigned line, ttype_t ttype, type_t *type_one, type_t *type_two)
{
  type_t *type = alloc(type_t);

#if DEBUG
  printf("new type operator (%d) in %s line %u\n", curr_oper_idd, file, line);
#else
  /* supress warnings */
  (void)file;
  (void)line;
#endif

  type->ttype = ttype;
  type->types[0] = type_one;
  type->types[1] = type_two;
  type->types_num = 2;
  type->idd = curr_oper_idd++; /* debug */

  return type;
}

type_t *type_integer(void)
{
  return type_operator_0(TYPE_INTEGER);
}

type_t *type_boolean(void)
{
  return type_operator_0(TYPE_BOOLEAN);
}

type_t *type_string(void)
{
  return type_operator_0(TYPE_STRING);
}

type_t *type_function(type_t *from, type_t *to)
{
  return type_operator_2(TYPE_FUNCTION, from, to);
}

type_t *type_pair(type_t *fst, type_t *snd)
{
  return type_operator_2(TYPE_PAIR, fst, snd);
}

bool is_type_variable(type_t *type)
{
  assert(type);

  if (type->ttype == TYPE_VARIABLE)
    return true;

  return false;
}

bool is_type_operator(type_t *type)
{
  assert(type);

  if (type->ttype == TYPE_VARIABLE)
    return false;

  return true;
}

void print_type(type_t *type)
{
  switch (type->ttype){
    case TYPE_VARIABLE:
      if (type->instance){
        print_type(type->instance);
      } else {
        if (!type->ch){
          printf("%c", type->ch = current_ch++);
        } else {
          printf("%c", type->ch);
        }
      }
      break;
    case TYPE_INTEGER:
      printf("int");
      break;
    case TYPE_BOOLEAN:
      printf("bool");
      break;
    case TYPE_STRING:
      printf("string");
      break;
    case TYPE_FUNCTION:
      printf("(");
      print_type(type->types[0]);
      printf(" -> ");
      print_type(type->types[1]);
      printf(")");
      break;
    case TYPE_PAIR:
      printf("(");
      print_type(type->types[0]);
      printf(" * ");
      print_type(type->types[1]);
      printf(")");
      break;
    default:
      fprintf(stderr, "#unknown_type!#print_type#\n");
      break;
  }
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

