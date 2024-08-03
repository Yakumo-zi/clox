#include "chunk.h"
#include "common.h"
#include "debug.h"

int main(int argc, char *argv[]) {
  Chunk chunk;
  init_chunk(&chunk);
  int idx = add_constants(&chunk, 1.2);
  write_chunk(&chunk, OP_CONSTANT);
  write_chunk(&chunk, idx);
  write_chunk(&chunk, OP_RETURN);
  disassemble_chunk(&chunk, "test");
  free_chunk(&chunk);
  return 0;
}
