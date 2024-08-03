#include "debug.h"
#include <stdio.h>
static int simple_instrunction(const char *name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

static int constant_instrunction(const char *name, Chunk *chunk, int offset) {
  int constant = 0;
  int offset_pos = 0;
  switch (chunk->code[offset]) {
  case OP_CONSTANT:
    constant = chunk->code[offset + 1];
    offset_pos = 2;
    break;
  case OP_CONSTANT_LONG:
    for (int i = 3; i >= 1; i--) {
      constant |= chunk->code[offset + i];
      constant <<= 8;
    }
    constant >>= 8;
    offset_pos = 4;
    break;
  }
  printf("%-16s %4d '", name, constant);
  print_value(chunk->constants.values[constant]);
  printf("'\n");
  return offset + offset_pos;
}

void disassemble_chunk(Chunk *chunk, const char *name) {
  printf("==%s==\n", name);
  for (int offset = 0; offset < chunk->count;) {
    offset = disassemble_instruction(chunk, offset);
  }
}
int disassemble_instruction(Chunk *chunk, int offset) {
  printf("%04d ", offset);
  if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
    printf("   | ");
  } else {
    printf("%4d ", chunk->lines[offset]);
  }
  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
  case OP_RETURN:
    return simple_instrunction("OP_RETURN", offset);
  case OP_CONSTANT:
    return constant_instrunction("OP_CONSTANT", chunk, offset);
  case OP_CONSTANT_LONG:
    return constant_instrunction("OP_CONSTANT_LONG", chunk, offset);
  default:
    printf("Unknown opcode %d\n", instruction);
    return offset + 1;
  }
}
