#include "vm.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"
#include "compiler.h"
#include <stdio.h>

VM vm;
static void reset_stack() { vm.stack_top = vm.stack; };
void init_vm() { reset_stack(); }
void free_vm() {}

void push(Value value) {
    *vm.stack_top = value;
    vm.stack_top++;
}
Value pop() {
    vm.stack_top--;
    return *vm.stack_top;
}

InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_CONSTANT_LONG(idx) (vm.chunk->constants.values[idx])
#define BINARY_OP(op)                                                          \
    do {                                                                       \
        *(vm.stack_top - 2) = ((*(vm.stack_top - 2)) op(*(vm.stack_top - 1))); \
        vm.stack_top -= 1;                                                     \
    } while (false)

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (Value *slot = vm.stack; slot < vm.stack_top; slot++) {
            printf("[");
            print_value(*slot);
            printf("]");
        }
        printf("\n");
        disassemble_instruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instrunction;
        switch (instrunction = READ_BYTE()) {
        case OP_RETURN:
#ifdef DEBUG_TRACE_EXECUTION
            printf("          ");
            printf("[");
            print_value(pop());
            printf("]\n");
#else
            print_value(pop());
            printf("\n");
#endif
            return INTERPRET_OK;
        case OP_CONSTANT:
            Value constant = READ_CONSTANT();
            push(constant);
            break;
        case OP_CONSTANT_LONG:
            // caculate long idx for constant
            int idx = READ_BYTE();
            idx += READ_BYTE() << 8;
            idx += READ_BYTE() << 16;
            Value constant_long = READ_CONSTANT_LONG(idx);
            push(constant_long);
            break;
        case OP_NEGATE:
            *(vm.stack_top - 1) = -(*(vm.stack_top - 1));
            break;
        case OP_ADD:
            BINARY_OP(+);
            break;
        case OP_SUB:
            BINARY_OP(-);
            break;
        case OP_MUL:
            BINARY_OP(*);
            break;
        case OP_DIV:
            BINARY_OP(/);
            break;
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_CONSTANT_LONG
}

InterpretResult interpret(const char* source) {
    compile(source);
    return INTERPRET_OK;
}
