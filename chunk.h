#pragma once

#include "common.h"
#include "value.h"

typedef enum { OP_RETURN, OP_CONSTANT, OP_CONSTANT_LONG } OpCode;

typedef struct {
  int count;
  int capacity;
  uint8_t *code;
  int *lines;
  ValueArray constants;
} Chunk;

void init_chunk(Chunk *chunk);

void free_chunk(Chunk *chunk);
void write_chunk(Chunk *chunk, uint8_t byte, int line);
void write_constant(Chunk *chunk, Value value, int line);
int add_constants(Chunk *chunk, Value value);
