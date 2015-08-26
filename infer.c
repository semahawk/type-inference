/*
 *
 * infer.c
 *
 * Created at:  Mon Aug 24 14:48:17 2015 14:48:17
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include "env.h"
#include "infer.h"
#include "node.h"
#include "type.h"

static bool debug = false;
static jmp_buf infer_jmp_buf;

bool is_generic(type_t *type, ng_t *non_generic)
{
  unsigned i;

#if DEBUG
  printf("non-generic(%p): {", (void *)non_generic);
#endif

  for (i = 0; i < non_generic->slots_num; i++){
#if DEBUG
    printf("%d ", non_generic->slots[i]->idd);
#endif

    if (occurs_in_type(type, non_generic->slots[i]))
      return false;
  }

#if DEBUG
  printf("}\n");
#endif

  return true;
}

type_t *freshrec(type_t *type, ng_t *non_generic, mapping_t *mappings, unsigned *current_mapping, unsigned *mappings_num)
{
  type_t *pruned = prune(type);

  if (is_type_variable(pruned)){
    if (is_generic(pruned, non_generic)){
#if DEBUG
      printf("type %d (instance %p) is a generic variable\n", pruned->idd, (void*)pruned->instance);
#endif

      unsigned i;
      mapping_t *ptr;
      bool found = false;

#if DEBUG
      printf("%d: mappings(%d): {", pruned->idd, *mappings_num);
#endif

      for (i = 0; i < *mappings_num; i++){
        ptr = &mappings[i];

#if DEBUG
        printf("%p(%d): %p(%d)", (void*)ptr->from, ptr->from->idd, (void*)ptr->to, ptr->to->idd);

        if (i != *mappings_num - 1)
          printf(", ");
#endif

        if (types_are_equal(ptr->from, pruned)){
          found = true;
          break;
        }
      }

#if DEBUG
      printf("}\n");
#endif

      if (!found){
#if DEBUG
        printf("type ");
        print_type(pruned);
        printf(" NOT in mappings\n");
#endif

        ptr = &mappings[*current_mapping];
        ptr->from = pruned;
        ptr->to = type_variable();

        (*current_mapping)++;
        (*mappings_num)++;
      } else {
#if DEBUG
        printf("type ");
        print_type(pruned);
        printf(" IS  in mappings\n");
#endif
      }

      return ptr->to;
    } else {
      /* not generic */
#if DEBUG
      printf("type %d (instance %p) is a non-generic variable\n", pruned->idd, (void*)pruned->instance);
#endif

      return pruned;
    }
  } else if (is_type_operator(pruned)){
#if DEBUG
    printf("type %d is an operator\n", pruned->idd);
#endif

    switch (pruned->types_num){
      case 0: return type_operator_0(pruned->ttype);
      case 1: return type_operator_1(pruned->ttype,
                  freshrec(pruned->types[0], non_generic, mappings, current_mapping, mappings_num));
      case 2: return type_operator_2(pruned->ttype,
                  freshrec(pruned->types[0], non_generic, mappings, current_mapping, mappings_num),
                  freshrec(pruned->types[1], non_generic, mappings, current_mapping, mappings_num));

      default:
        fprintf(stderr, "oy-vey, what types num is this?.\n");
        exit(1);
    }
  } else {
    fprintf(stderr, "oy-vey wat is dat.\n");
    exit(1);
  }
}

type_t *fresh(type_t *type, ng_t *non_generic)
{
  mapping_t mappings[256] = { { NULL, NULL } };
  unsigned current_mapping = 0;
  unsigned mappings_num = 0;

  return freshrec(type, non_generic, mappings, &current_mapping, &mappings_num);
}

bool types_are_equal(type_t *type1, type_t *type2)
{
  /* FIXME? */
  return type1 == type2;

  assert(type1 != NULL && type2 != NULL);

  if (type1->ttype != type2->ttype)
    return false;

  if (is_type_operator(type1)){
    unsigned i;

    if (type1->types_num != type2->types_num)
      return false;

    for (i = 0; i < type1->types_num; i++)
      if (!types_are_equal(type1->types[i], type2->types[i]))
        return false;
  }

  return true;
}

bool occurs_in_type(type_t *type1, type_t *type2)
{
  assert(type1 != NULL && type2 != NULL);

  type_t *pruned_type2 = prune(type2);

  if (is_type_variable(pruned_type2))
    return types_are_equal(type1, pruned_type2);
  else if (is_type_operator(pruned_type2))
    return occurs_in(type1, pruned_type2);
  else {
    fprintf(stderr, "neither a type operator nor a type variable\n");
    exit(1);
  }
}

bool occurs_in(type_t *type1, type_t *type2)
{
  assert(type1 != NULL && type2 != NULL);

  unsigned i;

  for (i = 0; i < type2->types_num; i++)
    if (occurs_in_type(type1, type2->types[i]))
      return true;

  return false;
}

type_t *prune(type_t *type)
{
  assert(type != NULL);

  if (is_type_variable(type))
    if (type->instance){
      type->instance = prune(type->instance);
      return type->instance;
    }

  return type;
}

void unify(type_t *type1, type_t *type2)
{
  /*printf("entering unify.\n");*/
  assert(type1 != NULL);
  assert(type2 != NULL);

  type_t *a = prune(type1);
  type_t *b = prune(type2);

  assert(a != NULL && b != NULL);

  if (is_type_variable(a)){
    /*printf("a is typeVARiable\n");*/
    if (!types_are_equal(a, b)){
      if (occurs_in_type(a, b)){
        printf("recursive unification");
        longjmp(infer_jmp_buf, 1);
      } else {
        /*print_type(a);*/
        /*printf("->instance (%d) = ", a->idd);*/
        /*print_type(b);*/
        /*printf("\n");*/

        a->instance = b;
      }
    }
  } else if (is_type_operator(a) && is_type_variable(b)){
    /*printf("a is typeOPERator, b is typeVARiable\n");*/
    unify(b, a);
  } else if (is_type_operator(a) && is_type_operator(b)){
    /*printf("a is typeOPERator, b is typeOPERator\n");*/
    /*print_type(a);*/
    /*printf(" (%d) vs ", a->idd);*/
    /*print_type(b);*/
    /*printf(" (%d)\n", b->idd);*/

    if ((a->ttype != b->ttype) || (a->types_num != b->types_num)){
      printf("type mismatch: ");
      print_type(a);
      printf(" != ");
      print_type(b);
      longjmp(infer_jmp_buf, 1);
    } else {
      type_t *p, *q;
      unsigned i;

      for (i = 0; i < a->types_num; i++){
        p = a->types[i];
        q = b->types[i];

        /*printf("unifying ");*/
        /*print_type(p);*/
        /*printf(" and ");*/
        /*print_type(q);*/
        /*printf("\n");*/
        unify(p, q);
      }
    }
  } else {
    printf("couldn't unify the two types");
    longjmp(infer_jmp_buf, 1);
  }
}

type_t *infer_type_internal(env_t *env, node_t *node, ng_t *non_generic)
{
  assert(node);

  if (non_generic == NULL)
    non_generic = new_non_generic();

  switch (node->type){
    case INTEGER: {
      return type_integer();
    }
    case IDENT: {
      type_t *type = symbol_lookup(env, node->name, non_generic);

      if (!type){
        printf("unknown symbol '%s'", node->name);
        longjmp(infer_jmp_buf, 1);
      }

      return type;
    }
    case LAMBDA: {
      type_t *arg_type = type_variable();
      type_t *result_type;
      env_t *new_env = copy_env(env);
      ng_t *new_non_generic = copy_non_generic(non_generic);

#if DEBUG
      printf("created a copy (%p) of non-generic (%p)\n", (void*)new_non_generic, (void*)non_generic);
#endif
      new_symbol(new_env, node->name, arg_type);
      add_to_non_generic(new_non_generic, arg_type);

      result_type = infer_type_internal(new_env, node->body, new_non_generic);

      if (result_type)
        return type_function(arg_type, result_type);
      else
        return NULL;
    }
    case APPLY: {
      type_t *fun_type = infer_type_internal(env, node->fn, non_generic);
      type_t *arg_type = infer_type_internal(env, node->arg, non_generic);
      type_t *result_type = type_variable();

      /*print_type(fun_type);*/
      /*printf("; ");*/
      /*print_type(arg_type);*/
      /*printf("; ");*/
      /*print_type(result_type);*/
      /*printf("; ");*/

      if (fun_type && arg_type){
        unify(type_function(arg_type, result_type), fun_type);
        return result_type;
      } else {
        return NULL;
      }
    }
    case LET: {
      type_t *def_type = infer_type_internal(env, node->def, non_generic);
      env_t *new_env = copy_env(env);

      new_symbol(new_env, node->name, def_type);

      if (def_type)
        return infer_type_internal(new_env, node->body, non_generic);
      else
        return NULL;
    }
    case LETREC: {
      type_t *new_type = type_variable();
      type_t *def_type;
      env_t *new_env = copy_env(env);
      ng_t *new_non_generic = copy_non_generic(non_generic);

      new_symbol(new_env, node->name, new_type);
      add_to_non_generic(new_non_generic, new_type);

      def_type = infer_type_internal(new_env, node->def, non_generic);

      if (def_type){
        unify(new_type, def_type);
        return infer_type_internal(new_env, node->body, non_generic);
      } else {
        return NULL;
      }
    }
    default:
      return NULL;
  }
}

type_t *infer_type(env_t *env, node_t *node)
{
  if (!setjmp(infer_jmp_buf))
    return infer_type_internal(env, node, NULL);
  else
    return NULL;
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

