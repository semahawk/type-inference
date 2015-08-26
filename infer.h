/*
 *
 * infer.h
 *
 * Created at:  Mon Aug 24 14:47:44 2015 14:47:44
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef INFER_H
#define INFER_H

#include <stdbool.h>

#include "node.h"
#include "type.h"
#include "env.h"

typedef struct {
  type_t *from;
  type_t *to;
} mapping_t;

type_t *infer_type_internal(env_t *env, node_t *node, ng_t *nongen);
type_t *infer_type(env_t *env, node_t *node);
type_t *fresh(type_t *type, ng_t *non_generic);
bool types_are_equal(type_t *one, type_t *two);
bool occurs_in_type(type_t *v, type_t *type2);
bool occurs_in(type_t *v, type_t *type2);
type_t *prune(type_t *type);
void unify(type_t *one, type_t *two);

#endif /* INFER_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

