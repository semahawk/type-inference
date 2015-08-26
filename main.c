/*
 *
 * main.c
 *
 * Created at:  Tue Aug 11 12:31:14 2015 12:31:14
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "env.h"
#include "infer.h"
#include "node.h"
#include "type.h"

int main(void)
{
  env_t *env = new_env();

  type_t *t_int = type_integer();
  type_t *t_bool = type_boolean();

  type_t *var1 = type_variable();
  type_t *var2 = type_variable();
  type_t *var3 = type_variable();

  new_symbol(env, "true", t_bool);
  new_symbol(env, "zero", type_function(t_int, t_bool));
  new_symbol(env, "pred", type_function(t_int, t_int));
  new_symbol(env, "pair", type_function(var1, type_function(var2, type_pair(var1, var2))));
  new_symbol(env, "cond", type_function(t_bool, type_function(var3, type_function(var3, var3)))),

  new_symbol(env, "+", type_function(t_int, type_function(t_int, t_int)));
  new_symbol(env, "*", type_function(t_int, type_function(t_int, t_int)));
  new_symbol(env, ">", type_function(t_int, type_function(t_int, t_bool)));
  new_symbol(env, "<", type_function(t_int, type_function(t_int, t_bool)));

  node_t *pair = apply(apply(ident("pair"), apply(ident("f"), integer(4))), apply(ident("f"), ident("true")));

  node_t *examples[] = {
    /* basically 2 + 2 */
    binop("+", integer(2), integer(3)),
    /* (2 + 2) * 2 */
    binop("*", integer(2), binop("+", integer(2), integer(2))),
    /* 2 > 3 */
    binop(">", integer(2), integer(3)),
    /* should fail */
    /* 5 * (7 < 11) */
    binop("*", integer(5), binop("<", integer(7), integer(11))),
    /* factorial */
    letrec("factorial", /* letrec factorial = */
        lambda("n",    /* fn n => */
            apply(
                apply(   /* cond (zero n) 1 */
                    apply(ident("cond"),     /* cond (zero n) */
                        apply(ident("zero"), ident("n"))),
                    integer(1)),
                apply(    /* times n */
                    apply(ident("times"), ident("n")),
                    apply(ident("factorial"),
                        apply(ident("pred"), ident("n")))
                )
            )
        ),      /* in */
        apply(ident("factorial"), integer(5))
    ),

    /* Should fail: */
    /* fn x => (pair(x(3) (x(true))) */
    lambda("x",
        apply(
            apply(ident("pair"),
                apply(ident("x"), integer(3))),
            apply(ident("x"), ident("true")))),

    /* pair(f(3), f(true)) */
    apply(
        apply(ident("pair"), apply(ident("f"), integer(4))),
        apply(ident("f"), ident("true"))),


    /* let f = (fn x => x) in ((pair (f 4)) (f true)) */
    let("f", lambda("x", ident("x")), pair),

    /* fn f => f f (fail) */
    lambda("f", apply(ident("f"), ident("f"))),

    /* let g = fn f => 5 in g g */
    let("g",
        lambda("f", integer(5)),
        apply(ident("g"), ident("g"))),

    /* example that demonstrates generic and non-generic variables: */
    /* fn g => let f = fn x => g in pair (f 3, f true) */
    lambda("g",
           let("f",
               lambda("x", ident("g")),
               apply(
                    apply(ident("pair"),
                          apply(ident("f"), integer(3))
                    ),
                    apply(ident("f"), ident("true"))))),

    /* Function composition */
    /* fn f (fn g (fn arg (f g arg))) */
    lambda("f", lambda("g", lambda("arg", apply(ident("g"), apply(ident("f"), ident("arg"))))))
  };

  unsigned i;
  node_t *node;
  type_t *inferred_type;

  for (i = 0; i < sizeof(examples) / sizeof(*examples); i++){
    node = examples[i];

    print_node(node);
    printf(" : ");

    if ((inferred_type = infer_type(env, node)) != NULL)
      print_type(inferred_type);

    printf("\n");
  }

  return 0;
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

