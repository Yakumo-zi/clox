#include "chunk.h"
#include "memmory.h"
#include "value.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void init_chunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->line_count = 0;
  chunk->line_capcaity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  init_value_array(&chunk->constants);
}

void write_chunk(Chunk *chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) {
    int old_capacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(old_capacity);
    chunk->code =
        GROW_ARRAY(uint8_t, chunk->code, old_capacity, chunk->capacity);
  }
  if (chunk->line_capcaity < chunk->line_count + 1) {
    int old_line_capacity = chunk->line_capcaity;
    chunk->line_capcaity = GROW_CAPACITY(old_line_capacity);
    chunk->lines = GROW_ARRAY(uint64_t, chunk->lines, old_line_capacity,
                              chunk->line_capcaity);
  }
  chunk->code[chunk->count] = byte;
  int last_line = chunk->line_count == 0 ? chunk->lines[chunk->line_count]
                                         : chunk->lines[chunk->line_count - 1];
  uint64_t line_info = chunk->count;
  line_info <<= 32;
  if (last_line != line) {
    line_info += line;
    chunk->lines[chunk->line_count] = line_info;
    chunk->line_count++;
  } else {
    line_info += (int)chunk->lines[chunk->line_count - 1];
    chunk->lines[chunk->line_count - 1] = line_info;
  }
  chunk->count++;
}

void write_constant(Chunk *chunk, Value value, int line) {
  int idx = add_constants(chunk, value);
  if (idx < 255) {
    write_chunk(chunk, OP_CONSTANT, line);
    write_chunk(chunk, idx & 0xFF, line);
  } else {
    write_chunk(chunk, OP_CONSTANT_LONG, line);
    for (int i = 0; i < 3; i++) {
      write_chunk(chunk, idx & 0xFF, line);
      idx >>= 8;
    }
  }
}

void free_chunk(Chunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  free_value_array(&chunk->constants);
  init_chunk(chunk);
}

int add_constants(Chunk *chunk, Value value) {
  write_value_array(&chunk->constants, value);
  return chunk->constants.count - 1;
}
