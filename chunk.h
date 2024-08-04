#pragma once

#include "value.h"

typedef enum { OP_RETURN, OP_CONSTANT, OP_CONSTANT_LONG,OP_ADD,OP_SUB,OP_MUL,OP_DIV, OP_NEGATE} OpCode;

typedef struct {
  int count;
  int capacity;
  int line_count;
  int line_capcaity;
  uint8_t *code;
  uint64_t *lines;
  ValueArray constants;
} Chunk;

void init_chunk(Chunk *chunk);

void free_chunk(Chunk *chunk);
void write_chunk(Chunk *chunk, uint8_t byte, int line);
void write_constant(Chunk *chunk, Value value, int line);
int add_constants(Chunk *chunk, Value value);
