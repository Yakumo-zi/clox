#include "debug.h"
#include "chunk.h"
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

int get_instrunction_line(Chunk *chunk, int idx) {
    for (int i = 0; i < chunk->line_count; i++) {
        int range = chunk->lines[i] >> 32;
        if (idx <= range) {
            return chunk->lines[i];
        }
    }
    return -1;
}

void disassemble_chunk(Chunk *chunk, const char *name) {
    printf("==%s==\n", name);
    for (int offset = 0; offset < chunk->count;) {
        offset = disassemble_instruction(chunk, offset);
    }
}
int disassemble_instruction(Chunk *chunk, int offset) {
    printf("%04d ", offset);
    static int last_line = 0;
    int line = get_instrunction_line(chunk, offset);
    if (last_line == line) {
        printf("   | ");
    } else {
        printf("%4d ", line);
        last_line = line;
    }
    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
    case OP_RETURN:
        return simple_instrunction("OP_RETURN", offset);
    case OP_CONSTANT:
        return constant_instrunction("OP_CONSTANT", chunk, offset);
    case OP_CONSTANT_LONG:
        return constant_instrunction("OP_CONSTANT_LONG", chunk, offset);
    case OP_NEGATE:
        return simple_instrunction("OP_NEGATE", offset);
    case OP_ADD:
        return simple_instrunction("OP_ADD", offset);
    case OP_SUB:
        return simple_instrunction("OP_SUB", offset);
    case OP_MUL:
        return simple_instrunction("OP_MUL", offset);
    case OP_DIV:
        return simple_instrunction("OP_DIV", offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}
