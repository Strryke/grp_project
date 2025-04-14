#include "singlish_c.h"
#include "lexer/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to read entire file into memory
static char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate buffer and read file
    char* buffer = (char*)malloc(size + 1);
    if (buffer) {
        fread(buffer, 1, size, file);
        buffer[size] = '\0';
    }

    fclose(file);
    return buffer;
}

void print_error(const char* message, int line, int column) {
    fprintf(stderr, "Error at line %d, column %d: %s\n", line, column, message);
}

// Convert Singlish token to C equivalent
static void write_token(FILE* out, Token* token, Token* next_token, Token* prev_token) {
    // Special handling for function declarations
    if (token->type == TOKEN_IDENTIFIER && 
        prev_token && prev_token->type == TOKEN_KOPI_O &&
        next_token && next_token->type == TOKEN_LPAREN) {
        fprintf(out, "%s ", token->value); // Function name
        return;
    }

    switch (token->type) {
        case TOKEN_KOPI_O:
            fprintf(out, "int");
            break;
        case TOKEN_TEH_O:
            fprintf(out, "float");
            break;
        case TOKEN_CHAR_KWAY:
            fprintf(out, "char");
            break;
        case TOKEN_PRINT_LAH:
            fprintf(out, "printf");
            break;
        case TOKEN_WALAO:
            fprintf(out, "return");
            break;
        case TOKEN_LAH:
            fprintf(out, ";");
            break;
        case TOKEN_STRING:
            fprintf(out, "\"%s\"", token->value);
            break;
        case TOKEN_LPAREN:
            fprintf(out, "(");
            break;
        case TOKEN_RPAREN:
            fprintf(out, ")");
            break;
        case TOKEN_LBRACE:
            fprintf(out, "{");
            break;
        case TOKEN_RBRACE:
            fprintf(out, "}");
            break;
        case TOKEN_IDENTIFIER:
            fprintf(out, "%s", token->value);
            break;
        case TOKEN_NUMBER:
            fprintf(out, "%s", token->value);
            break;
        default:
            // Skip unknown tokens
            break;
    }

    // Add space after tokens except special cases
    if (token->type != TOKEN_LAH && 
        token->type != TOKEN_LPAREN &&
        token->type != TOKEN_LBRACE &&
        (next_token && next_token->type != TOKEN_RPAREN && 
         next_token->type != TOKEN_LAH && 
         next_token->type != TOKEN_LBRACE)) {
        fprintf(out, " ");
    }
    
    // Add newline after certain tokens
    if (token->type == TOKEN_LAH ||
        token->type == TOKEN_LBRACE ||
        token->type == TOKEN_RBRACE) {
        fprintf(out, "\n");
    }
}

ErrorCode compile_file(const char* input_file, const char* output_file) {
    // Read input file
    char* source = read_file(input_file);
    if (!source) {
        print_error("Could not read input file", 0, 0);
        return ERROR_FILE_NOT_FOUND;
    }

    // Create lexer
    Lexer* lexer = lexer_create(source);
    
    // Open output file
    FILE* out = fopen(output_file, "w");
    if (!out) {
        free(source);
        lexer_destroy(lexer);
        return ERROR_FILE_NOT_FOUND;
    }

    // Write standard includes
    fprintf(out, "#include <stdio.h>\n\n");

    // Process tokens and generate C code
    Token* token = NULL;
    Token* next_token = NULL;
    Token* prev_token = NULL;

    // Get first token
    token = lexer_next_token(lexer);

    while (token != NULL) {
        if (token->type == TOKEN_EOF) {
            token_destroy(token);
            break;
        }

        // Look ahead for the next token
        next_token = lexer_next_token(lexer);
        
        write_token(out, token, next_token, prev_token);
        
        // Clean up previous token
        if (prev_token) {
            token_destroy(prev_token);
        }
        
        // Move tokens forward
        prev_token = token;
        token = next_token;
        next_token = NULL;
    }

    if (prev_token) {
        token_destroy(prev_token);
    }

    fclose(out);
    free(source);
    lexer_destroy(lexer);

    return SUCCESS;
} 