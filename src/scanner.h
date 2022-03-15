#ifndef SCANNER_H_
#define SCANNER_H_

#include <stdio.h>
#include <stdint.h>

#define SCANNER_MAX_TOKENS 4096

enum token_type_e {
    TK_ADD, TK_SUBTRACT, TK_MULTIPLY, TK_DIVIDE,
    TK_INT_LITERAL,
    TK_WHITESPACE, TK_EOF
};

struct token_t {
    int32_t type;
    int32_t value;
};

struct scanner_t {
    char* source;
    int32_t source_length;
    int32_t position;
    int32_t line;
    int32_t column;

    struct token_t* tokens;
    int32_t token_count;
};

int32_t scanner_init(struct scanner_t* scanner, const char* program_path);
int32_t scan_token(struct token_t* token, struct scanner_t* scanner);
void scan_integer_literal(struct token_t* token, struct scanner_t* scanner);
void scan_operator(struct token_t* token, struct scanner_t* scanner);

int32_t scanner_current_char(struct scanner_t* scanner);
int32_t scanner_next_char(struct scanner_t* scanner);

int is_digit(int c);
int is_operator(int c);
int is_whitechar(int c);

#endif
