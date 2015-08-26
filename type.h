/*
 *
 * type.h
 *
 * Created at:  Tue Aug 11 13:13:50 2015 13:13:50
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef TYPE_H
#define TYPE_H

#include <stdbool.h>

typedef enum {
  /* type variable */
  TYPE_VARIABLE,
  /* type operator */
  TYPE_INTEGER,
  TYPE_BOOLEAN,
  TYPE_FUNCTION,
  TYPE_PAIR
} ttype_t;

typedef struct _type {
  int idd; /* debug */
  /* for type_variable */
  size_t id; /* unique id */
  char ch; /* variable's name, assigned lazily */
  struct _type *instance;
  /* for type_operator (and it's derivatives) */
  ttype_t ttype;
  struct _type *types[2]; /* some types contain other types */
  unsigned types_num; /* how many types in the `types` field */
} type_t;

#define type_variable() type_variable_(__FILE__, __LINE__)
type_t *type_variable_(const char *, unsigned);

#define type_operator_0(t) type_operator_0_(__FILE__, __LINE__, t)
#define type_operator_1(t,u) type_operator_1_(__FILE__, __LINE__, t, u)
#define type_operator_2(t,u,v) type_operator_2_(__FILE__, __LINE__, t, u, v)
type_t *type_operator_0_(const char *, unsigned, ttype_t);
type_t *type_operator_1_(const char *, unsigned, ttype_t, type_t *type);
type_t *type_operator_2_(const char *, unsigned, ttype_t, type_t *type_one, type_t *type_two);

type_t *type_integer(void);
type_t *type_boolean(void);

type_t *type_function(type_t *in, type_t *out);
type_t *type_pair(type_t *fst, type_t *snd);

bool is_type_variable(type_t *type);
bool is_type_operator(type_t *type);

void print_type(type_t *type);

#endif /* TYPE_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

