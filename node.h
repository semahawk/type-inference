/*
 *
 * node.h
 *
 * Created at:  Tue Aug 11 12:28:23 2015 12:28:23
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef NODE_H
#define NODE_H

enum node_type {
  INTEGER,
  LAMBDA,
  IDENT,
  APPLY,
  LET,
  LETREC
};

typedef struct _node {
  enum node_type type;
  /* we really don't care about space efficency */
  /* integer */
  int value;
  /* common */
  const char *name;
  struct _node *body;
  /* lambda */
  /*   name, body */
  /* ident */
  /*   name */
  /* apply */
  struct _node *fn;
  struct _node *arg;
  /* let / letrec */
  struct _node *def;
  /*   name, body */
} node_t;

node_t *integer(int value);
node_t *lambda(const char *param, node_t *body);
node_t *ident(const char *name);
node_t *let   (const char *param, node_t *def, node_t *body);
node_t *letrec(const char *param, node_t *def, node_t *body);
node_t *apply(node_t *fn, node_t *arg);
node_t *binop(const char *op, node_t *lhs, node_t *rhs);

void print_node(node_t *node);

#endif /* NODE_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

