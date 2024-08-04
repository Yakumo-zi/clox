#include "scanner.h"
#include <ctype.h>
#include <string.h>
typedef struct {
    const char *start;
    const char *current;
    int line;
} Scanner;

Scanner scanner;

void init_scanner(const char *source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static bool is_at_end() { return *scanner.current == '\0'; }

static Token make_token(TokenType type) {
    Token tok;
    tok.type = type;
    tok.length = scanner.current - scanner.start;
    tok.start = scanner.start;
    tok.line = scanner.line;
    return tok;
}

static Token error_token(const char *msg) {
    Token tok;
    tok.type = TOKEN_ERROR;
    tok.length = strlen(msg);
    tok.start = msg;
    tok.line = scanner.line;
    return tok;
}

static char advance() {
    scanner.current++;
    // get last char,*(scanner.current-1)
    return scanner.current[-1];
}

static bool match(char expected) {
    if (is_at_end())
        return false;
    if (*scanner.current != expected)
        return false;
    scanner.current++;
    return true;
}

static char peek() { return *scanner.current; }
static char peek_next() {
    if (is_at_end()) {
        return '\0';
    }
    return scanner.current[1];
}
static void skip_white_space() {
    for (;;) {
        char c = peek();
        switch (c) {
        case ' ':
        case '\r':
        case '\t':
            advance();
            break;
        case '\n':
            scanner.line++;
            advance();
            break;
        case '/':
            if (peek_next() == '/') {
                while (peek() != '\n' && !is_at_end()) {
                    advance();
                }
            } else {
                return;
            }
            break;
        default:
            return;
        }
    }
}
static Token string() {
    while (peek() != '"' && !is_at_end()) {
        if (peek() == '\n')
            scanner.line++;
        advance();
    }
    if (is_at_end())
        return error_token("Unterminated string.");
    advance();
    return make_token(TOKEN_STRING);
}

static Token number() {
    while (isdigit(peek())) {
        advance();
    }
    if (peek() == '.' && isdigit(peek_next())) {
        advance();
        while (isdigit(peek())) {
            advance();
        }
    }
    return make_token(TOKEN_NUMBER);
}

static TokenType check_keyword(int start, int length, const char *rest,
                               TokenType type) {
    if (scanner.current - scanner.start == start + length &&
        memcmp(scanner.start + start, rest, length) == 0) {
        return type;
    }
    return TOKEN_IDENTIFIER;
}

static TokenType identifier_type() {
    switch (scanner.start[0]) {
    case 'a':
        return check_keyword(1, 2, "nd", TOKEN_AND);
    case 'c':
        return check_keyword(1, 4, "lass", TOKEN_CLASS);
    case 'e':
        return check_keyword(1, 3, "lse", TOKEN_ELSE);
    case 'i':
        return check_keyword(1, 1, "f", TOKEN_IF);
    case 'n':
        return check_keyword(1, 2, "il", TOKEN_NIL);
    case 'o':
        return check_keyword(1, 1, "r", TOKEN_OR);
    case 'p':
        return check_keyword(1, 4, "rint", TOKEN_PRINT);
    case 'r':
        return check_keyword(1, 5, "eturn", TOKEN_RETURN);
    case 's':
        return check_keyword(1, 4, "uper", TOKEN_SUPER);
    case 'v':
        return check_keyword(1, 2, "ar", TOKEN_VAR);
    case 'w':
        return check_keyword(1, 4, "hile", TOKEN_WHILE);
    case 'f':
        if (scanner.current - scanner.start > 1) {
            switch (scanner.start[1]) {
            case 'a':
                return check_keyword(2, 3, "lse", TOKEN_FALSE);
            case 'o':
                return check_keyword(2, 1, "r", TOKEN_FOR);
            case 'u':
                return check_keyword(2, 1, "n", TOKEN_FUN);
            }
        }
        break;
    case 't':
      if (scanner.current - scanner.start > 1) {
        switch (scanner.start[1]) {
          case 'h': return check_keyword(2, 2, "is", TOKEN_THIS);
          case 'r': return check_keyword(2, 2, "ue", TOKEN_TRUE);
        }
      }
      break;
    }
    return TOKEN_IDENTIFIER;
}

static Token identifier() {
    while (isalpha(peek()) || isdigit(peek())) {
        advance();
    }
    return make_token(identifier_type());
}
Token scan_token() {
    skip_white_space();
    scanner.start = scanner.current;
    if (is_at_end())
        return make_token(TOKEN_EOF);
    char c = advance();
    if (isalpha(c)) {
        return identifier();
    }
    if (isdigit(c)) {
        return number();
    }
    switch (c) {
    case '(':
        return make_token(TOKEN_LEFT_PAREN);
    case ')':
        return make_token(TOKEN_RIGHT_PAREN);
    case '{':
        return make_token(TOKEN_LEFT_BRACE);
    case '}':
        return make_token(TOKEN_RIGHT_BRACE);
    case ';':
        return make_token(TOKEN_SEMICOLON);
    case ',':
        return make_token(TOKEN_COMMA);
    case '.':
        return make_token(TOKEN_DOT);
    case '-':
        return make_token(TOKEN_MINUS);
    case '+':
        return make_token(TOKEN_PLUS);
    case '/':
        return make_token(TOKEN_SLASH);
    case '*':
        return make_token(TOKEN_STAR);
    case '!':
        return make_token(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '>':
        return make_token(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '<':
        return make_token(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '=':
        return make_token(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '"':
        return string();
    }
    return error_token("unexpected character.");
}