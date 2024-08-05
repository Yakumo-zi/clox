#pragma once
#include "chunk.h"

void disassemble_chunk(Chunk *chunk, const char *name);
int disassemble_instruction(Chunk *chunk, int offset);
int get_instrunction_line(Chunk *chunk, int idx);
