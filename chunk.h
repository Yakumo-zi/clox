#pragma once

#include "common.h"

typedef enum { OP_RETURN } OpCode;

typedef struct {
  int count;
  int capacity;
  uint8_t *code;
} Chunk;

void init_chunk(Chunk *chunk);

void free_chunck(Chunk *chunk);
void write_chunck(Chunk *chunk, uint8_t byte);
