/*
 *
 * mem.h
 *
 * Created at:  Mon Aug 24 13:46:07 2015 13:46:07
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef MEM_H
#define MEM_H

#include <assert.h>
#include <stdlib.h>

static inline void *alloc_(size_t sz)
{
  void *allocated_memory = malloc(sz);

  assert(allocated_memory != NULL);

  return allocated_memory;
}

static inline void *allocz_(size_t sz)
{
  void *allocated_memory = calloc(1, sz);

  assert(allocated_memory != NULL);

  return allocated_memory;
}

#define alloc(t)  alloc_(sizeof(t))
#define allocz(t) allocz_(sizeof(t))

#endif /* MEM_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

