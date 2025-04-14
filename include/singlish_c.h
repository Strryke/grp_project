#ifndef SINGLISH_C_H
#define SINGLISH_C_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Error codes
typedef enum {
    SUCCESS = 0,
    ERROR_FILE_NOT_FOUND,
    ERROR_SYNTAX,
    ERROR_SEMANTIC,
    ERROR_CODEGEN
} ErrorCode;

// Token types
typedef enum {
    TOKEN_EOF = 0,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_LAH,          // lah (statement terminator)
    TOKEN_KOPI_O,       // int
    TOKEN_TEH_O,        // float
    TOKEN_CHAR_KWAY,    // char
    TOKEN_CAN,          // if
    TOKEN_CANNOT,       // else
    TOKEN_WALAO,        // return
    TOKEN_PRINT_LAH,    // printf
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_LBRACE,       // {
    TOKEN_RBRACE,       // }
    TOKEN_COMMA,        // ,
    TOKEN_SEMICOLON,    // ;
    TOKEN_EQUALS,       // =
    TOKEN_PLUS,         // +
    TOKEN_MINUS,        // -
    TOKEN_STAR,         // *
    TOKEN_SLASH,        // /
} TokenType;

// Function declarations
ErrorCode compile_file(const char* input_file, const char* output_file);
void print_error(const char* message, int line, int column);

#endif // SINGLISH_C_H 