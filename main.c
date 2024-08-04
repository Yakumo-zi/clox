#include "chunk.h"
#include "vm.h"

int main(int argc, char *argv[]) {
  init_vm();
  Chunk chunk;
  init_chunk(&chunk);
  write_constant(&chunk, 1.2, 123);
  write_chunk(&chunk, OP_NEGATE, 123 + 255);
  write_chunk(&chunk, OP_RETURN, 124);
  // disassemble_chunk(&chunk, "test");
  interpret(&chunk);
  free_vm();
  free_chunk(&chunk);
  return 0;
}
