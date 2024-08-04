#include "chunk.h"
#include "vm.h"

int main(int argc, char *argv[]) {
    init_vm();
    Chunk chunk;
    init_chunk(&chunk);
    write_constant(&chunk, 1.2, 123);
    write_constant(&chunk, 1.3, 123);
    write_chunk(&chunk, OP_ADD, 123);
    write_constant(&chunk, 1.3, 123);
    write_chunk(&chunk, OP_SUB, 123);
    write_constant(&chunk, 1.5, 123);
    write_chunk(&chunk, OP_MUL, 123);
    write_constant(&chunk, 1.0, 123);
    write_chunk(&chunk, OP_DIV, 123);
    write_chunk(&chunk, OP_NEGATE, 123);
    write_chunk(&chunk, OP_RETURN, 124);
    interpret(&chunk);
    free_vm();
    free_chunk(&chunk);
    return 0;
}
