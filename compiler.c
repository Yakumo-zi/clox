#include "compiler.h"
#include "chunk.h"
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
struct Parser {
  Token current;
  Token previous;
  bool had_error;
  bool panic_mode;
};
typedef enum {
  PREC_NONE,
  PREC_ASSIGNMENT, // =
  PREC_OR,         // or
  PREC_AND,        // and
  PREC_EQUALITY,   // == !=
  PREC_COMPARISON, // < > <= >=
  PREC_TERM,       // + -
  PREC_FACTOR,     // * /
  PREC_UNARY,      // ! -
  PREC_CALL,       // . ()
  PREC_PRIMARY
} Precedence;

struct Parser parser;
Chunk *compiling_chunk;

static void expression();
static void parse_precedence(Precedence precedence);

static Chunk *current_chunk() { return compiling_chunk; }

static void error_at(Token *tok, const char *message) {
  if (parser.panic_mode)
    return;
  parser.panic_mode = true;
  fprintf(stderr, "[line %d] Error", tok->line);
  if (tok->type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (tok->type == TOKEN_ERROR) {

  } else {
    fprintf(stderr, " at '%.*s'", tok->length, tok->start);
  }
  fprintf(stderr, ": %s\n", message);
  parser.had_error = true;
}
static void error_at_current(const char *message) {
  error_at(&parser.current, message);
}

static void advance() {
  parser.previous = parser.current;
  for (;;) {
    parser.current = scan_token();
    if (parser.current.type != TOKEN_ERROR)
      break;
    error_at_current(parser.current.start);
  }
}

static void consume(TokenType ty, const char *message) {
  if (parser.current.type == ty) {
    advance();
    return;
  }
  error_at_current(message);
}

static void emit_byte(uint8_t byte) {
  write_chunk(current_chunk(), byte, parser.previous.line);
}
static void emit_bytes(uint8_t byte1, uint8_t byte2) {
  emit_byte(byte1);
  emit_byte(byte2);
}
static void emit_return() { emit_byte(OP_RETURN); }
static void emit_constant(Value value) {
  write_constant(current_chunk(), value, parser.previous.line);
}

static void end_compiler() { emit_return(); }
static void binary() {
  TokenType op_type = parser.previous.type;
  ParseRule *rule = get_rule(op_type);
  parse_precedence((Precedence)(rule->precedence + 1));
  switch (op_type) {
  case TOKEN_PLUS:
    emit_byte(OP_ADD);
    break;
  case TOKEN_MINUS:
    emit_byte(OP_SUB);
    break;
  case TOKEN_STAR:
    emit_byte(OP_MUL);
    break;
  case TOKEN_SLASH:
    emit_byte(OP_DIV);
    break;
  }
}
static void grouping() {
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}
static void number() {
  double value = strtod(parser.previous.start, NULL);
  emit_constant(value);
}
static void unary() {
  TokenType op_type = parser.previous.type;
  parse_precedence(PREC_UNARY);
  switch (op_type) {
  case TOKEN_MINUS:
    emit_byte(OP_NEGATE);
    break;
  default:
    return;
  }
}
static void parse_precedence(Precedence precedence) {}
static void expression() { parse_precedence(PREC_ASSIGNMENT); }
bool compile(const char *source, Chunk *chunk) {
  init_scanner(source);
  compiling_chunk = chunk;
  parser.had_error = false;
  parser.panic_mode = false;
  advance();
  expression();
  consume(TOKEN_EOF, "Expect end of expression .");
  end_compiler();
  return !parser.had_error;
}
