/*
 *
 * node.c
 *
 * Created at:  Tue Aug 11 12:20:53 2015 12:20:53
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "mem.h"

node_t *make_node(enum node_type type)
{
  node_t *node = alloc(node_t);

  node->type = type;

  return node;
}

node_t *integer(int value)
{
  node_t *node = make_node(INTEGER);

  node->value.integer = value;

  return node;
}

node_t *string(const char *value)
{
  node_t *node = make_node(STRING);

  node->value.string = value;

  return node;
}

node_t *lambda(const char *param, node_t *body)
{
  node_t *node = make_node(LAMBDA);

  node->name = param;
  node->body = body;

  return node;
}

node_t *ident(const char *name)
{
  node_t *node = make_node(IDENT);

  node->name = name;

  return node;
}

node_t *let(const char *name, node_t *def, node_t *body)
{
  node_t *node = make_node(LET);

  node->name = name;
  node->def  = def;
  node->body = body;

  return node;
}

node_t *letrec(const char *name, node_t *def, node_t *body)
{
  node_t *node = make_node(LETREC);

  node->name = name;
  node->def  = def;
  node->body = body;

  return node;
}

node_t *apply(node_t *fn, node_t *arg)
{
  node_t *node = make_node(APPLY);

  node->fn  = fn;
  node->arg = arg;

  return node;
}

node_t *call(node_t *fn, node_t *arg, ...)
{
  node_t *node = apply(fn, arg), *other_arg;
  va_list vl;

  va_start(vl, arg);

  while (1)
    if ((other_arg = va_arg(vl, node_t *)) == NULL)
      break;
    else
      node = apply(node, other_arg);

  va_end(vl);

  return node;
}

node_t *binop(const char *op, node_t *lhs, node_t *rhs)
{
  return apply(apply(ident(op), lhs), rhs);
}

void print_node(node_t *node)
{
  switch (node->type){
    case INTEGER:
      printf("%d", node->value.integer);
      break;
    case STRING:
      printf("\"%s\"", node->value.string);
      break;
    case LAMBDA:
      printf("(fun %s -> ", node->name);
      print_node(node->body);
      printf(")");
      break;
    case IDENT:
      printf("%s", node->name);
      break;
    case APPLY:
      printf("(");
      print_node(node->fn);
      printf(" ");
      print_node(node->arg);
      printf(")");
      break;
    case LET:
      printf("(let %s = ", node->name);
      print_node(node->def);
      printf(" in ");
      print_node(node->body);
      printf(")");
      break;
    case LETREC:
      printf("(letrec %s = ", node->name);
      print_node(node->def);
      printf(" in ");
      print_node(node->body);
      printf(")");
      break;

    default:
      printf("#unknown_node_type#print_node#");
      break;
  }
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

