#include "./lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static Token* create_token(Lexer* lexer, TokenType type, const char* value) {
    Token* token = (Token*)malloc(sizeof(Token));
    token->type = type;
    token->value = value ? strdup(value) : NULL;
    token->line = lexer->line;
    token->column = lexer->column;
    return token;
}

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

static char* read_number(Lexer* lexer) {
    int start = lexer->position;
    while (isdigit(lexer->source[lexer->position])) {
        lexer->position++;
        lexer->column++;
    }
    
    int length = lexer->position - start;
    char* value = (char*)malloc(length + 1);
    strncpy(value, &lexer->source[start], length);
    value[length] = '\0';
    return value;
}

static TokenType get_keyword_type(const char* identifier) {
    if (strcmp(identifier, "kopi_o") == 0) return TOKEN_KOPI_O;
    if (strcmp(identifier, "teh_o") == 0) return TOKEN_TEH_O;
    if (strcmp(identifier, "char_kway") == 0) return TOKEN_CHAR_KWAY;
    if (strcmp(identifier, "print_lah") == 0) return TOKEN_PRINT_LAH;
    if (strcmp(identifier, "walao") == 0) return TOKEN_WALAO;
    if (strcmp(identifier, "lah") == 0) return TOKEN_LAH;
    if (strcmp(identifier, "loop_until") == 0) return TOKEN_LOOP_UNTIL;
    if (strcmp(identifier, "for_what") == 0) return TOKEN_FOR_WHAT;
    if (strcmp(identifier, "can") == 0) return TOKEN_CAN;
    if (strcmp(identifier, "cannot") == 0) return TOKEN_CANNOT;
    if (strcmp(identifier, "array") == 0) return TOKEN_ARRAY;
    return TOKEN_IDENTIFIER;
}

Token* lexer_next_token(Lexer* lexer) {
    skip_whitespace(lexer);
    skip_comment(lexer);

    if (lexer->source[lexer->position] == '\0') {
        return create_token(lexer, TOKEN_EOF, NULL);
    }

    char current = lexer->source[lexer->position];
    char next = lexer->source[lexer->position + 1];

    // Handle array brackets
    if (current == '[') {
        lexer->position++;
        lexer->column++;
        return create_token(lexer, TOKEN_LBRACKET, "[");
    }
    if (current == ']') {
        lexer->position++;
        lexer->column++;
        return create_token(lexer, TOKEN_RBRACKET, "]");
    }

    // Handle numbers (for array indices)
    if (isdigit(current)) {
        char* value = read_number(lexer);
        return create_token(lexer, TOKEN_NUMBER, value);
    }

    // Handle identifiers (for array names)
    if (isalpha(current) || current == '_') {
        char* value = read_identifier(lexer);
        TokenType type = get_keyword_type(value);
        return create_token(lexer, type, value);
    }

    // Handle comparison operators
    if (current == '<' || current == '>' || current == '=' || current == '!') {
        if (next == '=') {
            // Two-character operators
            char* value = (char*)malloc(3);
            value[0] = current;
            value[1] = '=';
            value[2] = '\0';
            
            TokenType type;
            switch (current) {
                case '<': type = TOKEN_LESS_EQUAL; break;
                case '>': type = TOKEN_GREATER_EQUAL; break;
                case '=': type = TOKEN_EQUAL_EQUAL; break;
                case '!': type = TOKEN_NOT_EQUAL; break;
            }
            
            lexer->position += 2;
            lexer->column += 2;
            return create_token(lexer, type, value);
        } else {
            // Single-character operators
            char* value = (char*)malloc(2);
            value[0] = current;
            value[1] = '\0';
            
            TokenType type;
            switch (current) {
                case '<': type = TOKEN_LESS; break;
                case '>': type = TOKEN_GREATER; break;
                case '=': type = TOKEN_EQUALS; break;
            }
            
            lexer->position++;
            lexer->column++;
            return create_token(lexer, type, value);
        }
    }

    // Handle string literals
    if (current == '"') {
        char* value = read_string(lexer);
        if (!value) {
            return create_token(lexer, TOKEN_EOF, NULL);
        }
        return create_token(lexer, TOKEN_STRING, value);
    }

    // Handle single-character tokens
    lexer->position++;
    lexer->column++;
    
    switch (current) {
        case '(':
            return create_token(lexer, TOKEN_LPAREN, "(");
        case ')':
            return create_token(lexer, TOKEN_RPAREN, ")");
        case '{':
            return create_token(lexer, TOKEN_LBRACE, "{");
        case '}':
            return create_token(lexer, TOKEN_RBRACE, "}");
        case ',':
            return create_token(lexer, TOKEN_COMMA, ",");
        case '+':
            return create_token(lexer, TOKEN_PLUS, "+");
        case '-':
            return create_token(lexer, TOKEN_MINUS, "-");
        case '*':
            return create_token(lexer, TOKEN_STAR, "*");
        case '/':
            if (lexer->source[lexer->position] == '/') {
                // Comment found, skip it and continue lexing
                return lexer_next_token(lexer);
            }
            return create_token(lexer, TOKEN_SLASH, "/");
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return create_token(lexer, TOKEN_NUMBER, (char*)malloc(2));
        default:
            if (isspace(current)) {
                return lexer_next_token(lexer);
            }
            return create_token(lexer, TOKEN_EOF, NULL);
    }
}

void token_destroy(Token* token) {
    if (token->value) {
        free(token->value);
    }
    free(token);
} 