#include "./lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Lexer* lexer_create(const char* source) {
    Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
    lexer->source = source;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    return lexer;
}

void lexer_destroy(Lexer* lexer) {
    free(lexer);
}

static void skip_whitespace(Lexer* lexer) {
    while (lexer->source[lexer->position] != '\0' && 
           isspace(lexer->source[lexer->position])) {
        if (lexer->source[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

static void skip_comment(Lexer* lexer) {
    if (lexer->source[lexer->position] == '/' && 
        lexer->source[lexer->position + 1] == '/') {
        // Skip to end of line
        while (lexer->source[lexer->position] != '\0' && 
               lexer->source[lexer->position] != '\n') {
            lexer->position++;
            lexer->column++;
        }
    }
}

static char* read_identifier(Lexer* lexer) {
    int start = lexer->position;
    while (isalnum(lexer->source[lexer->position]) || 
           lexer->source[lexer->position] == '_') {
        lexer->position++;
        lexer->column++;
    }
    
    int length = lexer->position - start;
    char* value = (char*)malloc(length + 1);
    strncpy(value, &lexer->source[start], length);
    value[length] = '\0';
    return value;
}

static char* read_string(Lexer* lexer) {
    lexer->position++; // Skip opening quote
    lexer->column++;
    
    int start = lexer->position;
    while (lexer->source[lexer->position] != '"' && 
           lexer->source[lexer->position] != '\0') {
        lexer->position++;
        lexer->column++;
    }
    
    if (lexer->source[lexer->position] == '\0') {
        return NULL; // Unterminated string
    }
    
    int length = lexer->position - start;
    char* value = (char*)malloc(length + 1);
    strncpy(value, &lexer->source[start], length);
    value[length] = '\0';
    
    lexer->position++; // Skip closing quote
    lexer->column++;
    
    return value;
}

static TokenType get_keyword_type(const char* identifier) {
    if (strcmp(identifier, "kopi_o") == 0) return TOKEN_KOPI_O;
    if (strcmp(identifier, "teh_o") == 0) return TOKEN_TEH_O;
    if (strcmp(identifier, "char_kway") == 0) return TOKEN_CHAR_KWAY;
    if (strcmp(identifier, "print_lah") == 0) return TOKEN_PRINT_LAH;
    if (strcmp(identifier, "walao") == 0) return TOKEN_WALAO;
    if (strcmp(identifier, "lah") == 0) return TOKEN_LAH;
    return TOKEN_IDENTIFIER;
}

Token* lexer_next_token(Lexer* lexer) {
start:
    skip_whitespace(lexer);
    skip_comment(lexer);

    if (lexer->source[lexer->position] == '\0') {
        Token* token = (Token*)malloc(sizeof(Token));
        token->type = TOKEN_EOF;
        token->value = NULL;
        token->line = lexer->line;
        token->column = lexer->column;
        return token;
    }

    Token* token = (Token*)malloc(sizeof(Token));
    token->line = lexer->line;
    token->column = lexer->column;

    char current = lexer->source[lexer->position];

    // Handle string literals
    if (current == '"') {
        token->value = read_string(lexer);
        if (!token->value) {
            token->type = TOKEN_EOF;
            return token;
        }
        token->type = TOKEN_STRING;
        return token;
    }

    // Handle identifiers and keywords
    if (isalpha(current) || current == '_') {
        token->value = read_identifier(lexer);
        token->type = get_keyword_type(token->value);
        return token;
    }

    // Handle single-character tokens
    lexer->position++;
    lexer->column++;
    
    switch (current) {
        case '(':
            token->type = TOKEN_LPAREN;
            token->value = strdup("(");
            return token;
        case ')':
            token->type = TOKEN_RPAREN;
            token->value = strdup(")");
            return token;
        case '{':
            token->type = TOKEN_LBRACE;
            token->value = strdup("{");
            return token;
        case '}':
            token->type = TOKEN_RBRACE;
            token->value = strdup("}");
            return token;
        case ',':
            token->type = TOKEN_COMMA;
            token->value = strdup(",");
            return token;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            token->type = TOKEN_NUMBER;
            token->value = (char*)malloc(2);
            token->value[0] = current;
            token->value[1] = '\0';
            return token;
        case '/':
            if (lexer->source[lexer->position] == '/') {
                // Comment found, skip it and continue lexing
                free(token);
                goto start;
            }
            // Fall through to default if not a comment
        default:
            if (isspace(current)) {
                free(token);
                goto start;
            }
            token->type = TOKEN_EOF;
            token->value = NULL;
            return token;
    }
}

void token_destroy(Token* token) {
    if (token->value) {
        free(token->value);
    }
    free(token);
} 