/*
 *
 * env.h
 *
 * Created at:  Mon Aug 24 13:35:39 2015 13:35:39
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef ENV_H
#define ENV_H

#include "type.h"

typedef struct {
  const char *name;
  type_t *type;
} symbol_t;

typedef struct {
  symbol_t symbols[256];
  symbol_t *current_symbol;
  size_t symbols_num;
} env_t;

/* bleh.. */
typedef struct {
  type_t *slots[256];
  type_t **current_slot;
  size_t slots_num;
} ng_t;

env_t *new_env(void);
env_t *copy_env(env_t *env);
void new_symbol(env_t *env, const char *name, type_t *const type);
type_t *symbol_lookup(env_t *env, const char *name, ng_t *non_generic);

ng_t *new_non_generic(void);
ng_t *copy_non_generic(ng_t *);
void add_to_non_generic(ng_t *, type_t *);

#endif /* ENV_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

