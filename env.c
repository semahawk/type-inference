/*
 *
 * env.c
 *
 * Created at:  Mon Aug 24 13:36:13 2015 13:36:13
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env.h"
#include "infer.h"
#include "mem.h"
#include "type.h"

env_t *new_env(void)
{
  env_t *env = allocz(env_t);

  env->current_symbol = env->symbols;

  return env;
}

env_t *copy_env(env_t *env)
{
  env_t *new_env = alloc(env_t);

  memcpy(new_env, env, sizeof(env_t));
  new_env->current_symbol = new_env->symbols + env->symbols_num;

  return new_env;
}

void new_symbol(env_t *env, const char *name, type_t *const type)
{
  /* overflow? nah. */
  env->current_symbol->name = name;
  env->current_symbol->type = type;
  env->current_symbol++;
  env->symbols_num++;
}

type_t *symbol_lookup(env_t *env, const char *name, ng_t *non_generic)
{
  symbol_t *sym;

  for (sym = env->symbols; sym != env->current_symbol; sym++)
    if (!strcmp(sym->name, name)){
#if DEBUG
      printf("fetching fresh for %s (%d)\n", name, sym->type->idd);
#endif
      return fresh(sym->type, non_generic);
    }

  return NULL;
}

ng_t *new_non_generic(void)
{
  ng_t *ng = allocz(ng_t);

  ng->current_slot = ng->slots;

  return ng;
}

ng_t *copy_non_generic(ng_t *ng)
{
  /* nng = new non generics */
  ng_t *nng = alloc(ng_t);

  memcpy(nng, ng, sizeof(ng_t));
  nng->current_slot = nng->slots + ng->slots_num;

  return nng;
}

void add_to_non_generic(ng_t *ng, type_t *type)
{
  *ng->current_slot = type;
  ng->current_slot++;
  ng->slots_num++;
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

