#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// Forward declarations
static void advance(Parser* parser);
static bool match(Parser* parser, TokenType type);
static Node* parse_expression(Parser* parser);
static Node* parse_term(Parser* parser);
static Node* parse_factor(Parser* parser);
static Node* parse_array_declaration(Parser* parser);
static Node* parse_array_access(Parser* parser);
static Node* parse_statement(Parser* parser);

// Function implementations
static void advance(Parser* parser) {
    parser->previous = parser->current;
    parser->current = lexer_next_token(parser->lexer);
}

static bool match(Parser* parser, TokenType type) {
    if (parser->current->type == type) {
        advance(parser);
        return true;
    }
    return false;
}

Parser* parser_create(Lexer* lexer) {
    Parser* parser = (Parser*)malloc(sizeof(Parser));
    parser->lexer = lexer;
    parser->current = NULL;
    parser->previous = NULL;
    advance(parser); // Get first token
    return parser;
}

void parser_destroy(Parser* parser) {
    if (parser->current) {
        token_destroy(parser->current);
    }
    if (parser->previous) {
        token_destroy(parser->previous);
    }
    free(parser);
}

void node_destroy(Node* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_VARIABLE_DECL:
            free(node->as.var_decl.name);
            node_destroy(node->as.var_decl.initializer);
            break;
            
        case NODE_ARRAY_DECL:
            free(node->as.array_decl.name);
            if (node->as.array_decl.initializers) {
                for (int i = 0; i < node->as.array_decl.initializer_count; i++) {
                    node_destroy(node->as.array_decl.initializers[i]);
                }
                free(node->as.array_decl.initializers);
            }
            break;
            
        case NODE_ARRAY_ACCESS:
            free(node->as.array_access.array_name);
            node_destroy(node->as.array_access.index);
            break;
            
        case NODE_ASSIGNMENT:
            free(node->as.assignment.name);
            node_destroy(node->as.assignment.value);
            break;
            
        case NODE_BINARY_OP:
            node_destroy(node->as.binary_op.left);
            node_destroy(node->as.binary_op.right);
            break;
            
        case NODE_PRINT:
            free(node->as.print.format);
            for (int i = 0; i < node->as.print.arg_count; i++) {
                node_destroy(node->as.print.args[i]);
            }
            free(node->as.print.args);
            break;
            
        case NODE_IF:
            node_destroy(node->as.if_stmt.condition);
            node_destroy(node->as.if_stmt.then_branch);
            node_destroy(node->as.if_stmt.else_branch);
            break;
            
        case NODE_LOOP:
            node_destroy(node->as.loop.condition);
            node_destroy(node->as.loop.body);
            break;
            
        case NODE_BLOCK:
            for (int i = 0; i < node->as.block.statement_count; i++) {
                node_destroy(node->as.block.statements[i]);
            }
            free(node->as.block.statements);
            break;
            
        case NODE_IDENTIFIER:
            free(node->as.string);
            break;
            
        default:
            break;
    }
    
    free(node);
}

static Node* parse_term(Parser* parser) {
    Node* left = parse_factor(parser);
    if (!left) return NULL;
    
    while (parser->current->type == TOKEN_STAR || 
           parser->current->type == TOKEN_SLASH) {
        TokenType op = parser->current->type;
        advance(parser);
        
        Node* right = parse_factor(parser);
        if (!right) {
            node_destroy(left);
            return NULL;
        }
        
        Node* node = (Node*)malloc(sizeof(Node));
        node->type = NODE_BINARY_OP;
        node->as.binary_op.op = op;
        node->as.binary_op.left = left;
        node->as.binary_op.right = right;
        left = node;
    }
    
    return left;
}

static Node* parse_expression(Parser* parser) {
    Node* left = parse_term(parser);
    if (!left) return NULL;
    
    while (parser->current->type == TOKEN_PLUS || 
           parser->current->type == TOKEN_MINUS) {
        TokenType op = parser->current->type;
        advance(parser);
        
        Node* right = parse_term(parser);
        if (!right) {
            node_destroy(left);
            return NULL;
        }
        
        Node* node = (Node*)malloc(sizeof(Node));
        node->type = NODE_BINARY_OP;
        node->as.binary_op.op = op;
        node->as.binary_op.left = left;
        node->as.binary_op.right = right;
        left = node;
    }
    
    return left;
}

static Node* parse_array_declaration(Parser* parser) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NODE_ARRAY_DECL;
    
    // Get array type
    node->as.array_decl.array_type = parser->previous->type;
    
    // Get array name
    if (parser->current->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Expected array name\n");
        free(node);
        return NULL;
    }
    node->as.array_decl.name = strdup(parser->current->value);
    advance(parser);
    
    // Get array size
    if (!match(parser, TOKEN_LBRACKET)) {
        fprintf(stderr, "Expected '[' after array name\n");
        free(node->as.array_decl.name);
        free(node);
        return NULL;
    }
    
    if (parser->current->type != TOKEN_NUMBER) {
        fprintf(stderr, "Expected array size\n");
        free(node->as.array_decl.name);
        free(node);
        return NULL;
    }
    node->as.array_decl.size = atoi(parser->current->value);
    advance(parser);
    
    if (!match(parser, TOKEN_RBRACKET)) {
        fprintf(stderr, "Expected ']' after array size\n");
        free(node->as.array_decl.name);
        free(node);
        return NULL;
    }
    
    // Parse initializers if present
    node->as.array_decl.initializers = NULL;
    node->as.array_decl.initializer_count = 0;
    
    if (match(parser, TOKEN_EQUALS)) {
        if (!match(parser, TOKEN_LBRACE)) {
            fprintf(stderr, "Expected '{' for array initializers\n");
            free(node->as.array_decl.name);
            free(node);
            return NULL;
        }
        
        // Count initializers
        int count = 0;
        Token* save = parser->current;
        while (parser->current->type != TOKEN_RBRACE && 
               parser->current->type != TOKEN_EOF) {
            if (parser->current->type == TOKEN_COMMA) {
                count++;
            }
            advance(parser);
        }
        count++; // Last element
        
        // Allocate space for initializers
        node->as.array_decl.initializers = (Node**)malloc(sizeof(Node*) * count);
        node->as.array_decl.initializer_count = count;
        
        // Parse initializers
        int i = 0;
        parser->current = save;
        while (parser->current->type != TOKEN_RBRACE && 
               parser->current->type != TOKEN_EOF) {
            node->as.array_decl.initializers[i] = parse_expression(parser);
            i++;
            if (parser->current->type == TOKEN_COMMA) {
                advance(parser);
            }
        }
        
        if (!match(parser, TOKEN_RBRACE)) {
            fprintf(stderr, "Expected '}' after array initializers\n");
            for (int j = 0; j < i; j++) {
                node_destroy(node->as.array_decl.initializers[j]);
            }
            free(node->as.array_decl.initializers);
            free(node->as.array_decl.name);
            free(node);
            return NULL;
        }
    }
    
    return node;
}

static Node* parse_array_access(Parser* parser) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NODE_ARRAY_ACCESS;
    
    // Get array name
    node->as.array_access.array_name = strdup(parser->previous->value);
    
    // Parse index expression
    if (!match(parser, TOKEN_LBRACKET)) {
        fprintf(stderr, "Expected '[' for array access\n");
        free(node->as.array_access.array_name);
        free(node);
        return NULL;
    }
    
    node->as.array_access.index = parse_expression(parser);
    
    if (!match(parser, TOKEN_RBRACKET)) {
        fprintf(stderr, "Expected ']' after array index\n");
        node_destroy(node->as.array_access.index);
        free(node->as.array_access.array_name);
        free(node);
        return NULL;
    }
    
    return node;
}

static Node* parse_factor(Parser* parser) {
    if (parser->current->type == TOKEN_NUMBER) {
        Node* node = (Node*)malloc(sizeof(Node));
        node->type = NODE_NUMBER;
        node->as.number = atoi(parser->current->value);
        advance(parser);
        return node;
    }
    
    if (parser->current->type == TOKEN_IDENTIFIER) {
        advance(parser);
        if (parser->current->type == TOKEN_LBRACKET) {
            return parse_array_access(parser);
        }
        Node* node = (Node*)malloc(sizeof(Node));
        node->type = NODE_IDENTIFIER;
        node->as.string = strdup(parser->previous->value);
        return node;
    }
    
    if (parser->current->type == TOKEN_LPAREN) {
        advance(parser);
        Node* expr = parse_expression(parser);
        if (!match(parser, TOKEN_RPAREN)) {
            fprintf(stderr, "Expected ')' after expression\n");
            node_destroy(expr);
            return NULL;
        }
        return expr;
    }
    
    fprintf(stderr, "Expected expression\n");
    return NULL;
}

static Node* parse_statement(Parser* parser) {
    if (parser->current->type == TOKEN_KOPI_O || 
        parser->current->type == TOKEN_TEH_O || 
        parser->current->type == TOKEN_CHAR_KWAY) {
        TokenType type = parser->current->type;
        advance(parser);
        
        if (parser->current->type == TOKEN_IDENTIFIER) {
            advance(parser);
            if (parser->current->type == TOKEN_LBRACKET) {
                return parse_array_declaration(parser);
            }
            // Handle variable declaration
            Node* node = (Node*)malloc(sizeof(Node));
            node->type = NODE_VARIABLE_DECL;
            node->as.var_decl.var_type = type;
            node->as.var_decl.name = strdup(parser->previous->value);
            node->as.var_decl.initializer = NULL;
            
            if (match(parser, TOKEN_EQUALS)) {
                node->as.var_decl.initializer = parse_expression(parser);
                if (!node->as.var_decl.initializer) {
                    free(node->as.var_decl.name);
                    free(node);
                    return NULL;
                }
            }
            
            return node;
        }
    }
    
    // Handle other statement types
    return parse_expression(parser);
}

Node* parse_program(Parser* parser) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NODE_PROGRAM;
    
    // Allocate space for statements
    int capacity = 8;
    node->as.block.statements = (Node**)malloc(sizeof(Node*) * capacity);
    node->as.block.statement_count = 0;
    
    // Parse statements until EOF
    while (parser->current->type != TOKEN_EOF) {
        Node* stmt = parse_statement(parser);
        if (!stmt) {
            // Error occurred
            for (int i = 0; i < node->as.block.statement_count; i++) {
                node_destroy(node->as.block.statements[i]);
            }
            free(node->as.block.statements);
            free(node);
            return NULL;
        }
        
        // Resize array if needed
        if (node->as.block.statement_count >= capacity) {
            capacity *= 2;
            node->as.block.statements = (Node**)realloc(
                node->as.block.statements, 
                sizeof(Node*) * capacity
            );
        }
        
        // Add statement to program
        node->as.block.statements[node->as.block.statement_count++] = stmt;
        
        // Expect statement terminator
        if (!match(parser, TOKEN_LAH)) {
            fprintf(stderr, "Expected 'lah' after statement\n");
            for (int i = 0; i < node->as.block.statement_count; i++) {
                node_destroy(node->as.block.statements[i]);
            }
            free(node->as.block.statements);
            free(node);
            return NULL;
        }
    }
    
    return node;
} 