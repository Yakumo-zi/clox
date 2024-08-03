#include "chunk.h"
#include "common.h"
#include "debug.h"

int main(int argc, char *argv[]) {
  Chunk chunk;
  init_chunk(&chunk);
  for (int i = 0; i <= 255; i++) {
    write_constant(&chunk, 1.2 + i, 123);
  }
  write_chunk(&chunk, OP_RETURN, 123);
  disassemble_chunk(&chunk, "test");
  free_chunk(&chunk);
  return 0;
}
