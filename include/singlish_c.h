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
    // Keywords
    TOKEN_KOPI_O,       // int
    TOKEN_TEH_O,        // float
    TOKEN_CHAR_KWAY,    // char
    TOKEN_LAH,          // lah (statement terminator)
    TOKEN_CAN,          // if
    TOKEN_CANNOT,       // else
    TOKEN_WALAO,        // return
    TOKEN_PRINT_LAH,    // printf
    TOKEN_LOOP_UNTIL,   // while
    TOKEN_FOR_WHAT,     // for
    TOKEN_ARRAY,        // array
    
    // Operators
    TOKEN_EQUALS,       // =
    TOKEN_PLUS,         // +
    TOKEN_MINUS,        // -
    TOKEN_STAR,         // *
    TOKEN_SLASH,        // /
    TOKEN_LESS,         // <
    TOKEN_GREATER,      // >
    TOKEN_LESS_EQUAL,   // <=
    TOKEN_GREATER_EQUAL,// >=
    TOKEN_EQUAL_EQUAL,  // ==
    TOKEN_NOT_EQUAL,    // !=
    
    // Punctuation
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_LBRACE,       // {
    TOKEN_RBRACE,       // }
    TOKEN_LBRACKET,     // [
    TOKEN_RBRACKET,     // ]
    TOKEN_COMMA,        // ,
    TOKEN_SEMICOLON,    // ;
    
    // Literals
    TOKEN_NUMBER,       // 123
    TOKEN_STRING,       // "hello"
    TOKEN_IDENTIFIER,   // variable names
    
    // Special
    TOKEN_EOF,          // end of file
    TOKEN_ERROR         // error token
} TokenType;

// Function declarations
ErrorCode compile_file(const char* input_file, const char* output_file);
void print_error(const char* message, int line, int column);

#endif // SINGLISH_C_H 