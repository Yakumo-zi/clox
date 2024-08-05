#include "compiler.h"
#include "scanner.h"
#include <stdio.h>

bool compile(const char *source, Chunk *chunk) {
    init_scanner(source);
    // advance();
    // expression();
    // consume(TOKEN_EOF,"Expect end of expression .");
    int line = -1;
    for (;;) {
        Token token = scan_token();
        if (token.line != line) {
            printf("%04d", token.line);
            line = token.line;
        } else {
            printf("   |");
        }
        printf(" %2d '%.*s'\n", token.type, token.length, token.start);
        if (token.type == TOKEN_EOF)
            break;
    }
    return true;
}