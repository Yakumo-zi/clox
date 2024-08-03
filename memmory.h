#pragma once
#include "common.h"

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)
#ifdef DEBUG
#define GROW_ARRAY(type, pointer, old_count, new_count)                        \
  (printf("%s,%d,%d\n", #type, old_count, new_count),                          \
   (type *)reallocate(pointer, sizeof(type) * (old_count),                     \
                      sizeof(type) * (new_count)))
#else

#define GROW_ARRAY(type, pointer, old_count, new_count)                        \
  (type *)reallocate(pointer, sizeof(type) * (old_count),                      \
                     sizeof(type) * (new_count))
#endif
#define FREE_ARRAY(type, pointer, capacity)                                    \
  reallocate(pointer, sizeof(type) * capacity, 0)
void *reallocate(void *pointer, size_t old_size, size_t new_size);
