#ifndef LEXER_H
#define LEXER_H

#include "singlish_c.h"

typedef struct {
    TokenType type;
    char* value;
    int line;
    int column;
} Token;

typedef struct {
    const char* source;
    int position;
    int line;
    int column;
} Lexer;

Lexer* lexer_create(const char* source);
void lexer_destroy(Lexer* lexer);
Token* lexer_next_token(Lexer* lexer);
void token_destroy(Token* token);

#endif // LEXER_H 