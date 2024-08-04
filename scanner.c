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

static Token number(){
    while(isdigit(peek())){
        advance();
    }
    if(peek()=='.' && isdigit(peek_next())){
        advance();
        while(isdigit(peek())){
            advance();
        }
    }
    return make_token(TOKEN_NUMBER);
}
static TokenType identifier_type(){
    return TOKEN_IDENTIFIER;
}
static Token identifier(){
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
    if(isalpha(c)){
        return identifier();
    }
    if(isdigit(c)){
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