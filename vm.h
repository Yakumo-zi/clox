#pragma once

#include "chunk.h"
#define STACK_MAX 1024

typedef struct {
    Chunk *chunk;
    uint8_t *ip;
    Value stack[STACK_MAX];
    Value *stack_top;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILER_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;
void init_vm();
void free_vm();

InterpretResult interpret(const char *source);
void push(Value value);
Value pop();
