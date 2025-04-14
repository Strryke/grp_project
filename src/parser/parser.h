#ifndef PARSER_H
#define PARSER_H

#include "../lexer/lexer.h"

typedef enum {
    NODE_PROGRAM,
    NODE_FUNCTION,
    NODE_VARIABLE_DECL,
    NODE_ARRAY_DECL,
    NODE_ARRAY_ACCESS,
    NODE_ASSIGNMENT,
    NODE_BINARY_OP,
    NODE_NUMBER,
    NODE_IDENTIFIER,
    NODE_PRINT,
    NODE_RETURN,
    NODE_IF,
    NODE_LOOP,
    NODE_BLOCK
} NodeType;

typedef struct Node {
    NodeType type;
    union {
        // Variable declaration
        struct {
            TokenType var_type;
            char* name;
            struct Node* initializer;
        } var_decl;
        
        // Array declaration
        struct {
            TokenType array_type;
            char* name;
            int size;
            struct Node** initializers;
            int initializer_count;
        } array_decl;
        
        // Array access
        struct {
            char* array_name;
            struct Node* index;
        } array_access;
        
        // Assignment
        struct {
            char* name;
            struct Node* value;
        } assignment;
        
        // Binary operation
        struct {
            TokenType op;
            struct Node* left;
            struct Node* right;
        } binary_op;
        
        // Print statement
        struct {
            char* format;
            struct Node** args;
            int arg_count;
        } print;
        
        // If statement
        struct {
            struct Node* condition;
            struct Node* then_branch;
            struct Node* else_branch;
        } if_stmt;
        
        // Loop statement
        struct {
            struct Node* condition;
            struct Node* body;
        } loop;
        
        // Block
        struct {
            struct Node** statements;
            int statement_count;
        } block;
        
        // Literals
        int number;
        char* string;
    } as;
} Node;

typedef struct {
    Lexer* lexer;
    Token* current;
    Token* previous;
} Parser;

Parser* parser_create(Lexer* lexer);
void parser_destroy(Parser* parser);
Node* parse_program(Parser* parser);
void node_destroy(Node* node);

#endif // PARSER_H 