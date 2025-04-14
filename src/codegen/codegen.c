#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void generate_expression(FILE* output, Node* node);
static void generate_statement(FILE* output, Node* node);

static void generate_array_declaration(FILE* output, Node* node) {
    // Generate array declaration
    switch (node->as.array_decl.array_type) {
        case TOKEN_KOPI_O:
            fprintf(output, "int %s[%d]", node->as.array_decl.name, node->as.array_decl.size);
            break;
        case TOKEN_TEH_O:
            fprintf(output, "float %s[%d]", node->as.array_decl.name, node->as.array_decl.size);
            break;
        case TOKEN_CHAR_KWAY:
            fprintf(output, "char %s[%d]", node->as.array_decl.name, node->as.array_decl.size);
            break;
        default:
            fprintf(stderr, "Invalid array type\n");
            return;
    }
    
    // Generate initializers if present
    if (node->as.array_decl.initializers != NULL) {
        fprintf(output, " = {");
        for (int i = 0; i < node->as.array_decl.initializer_count; i++) {
            generate_expression(output, node->as.array_decl.initializers[i]);
            if (i < node->as.array_decl.initializer_count - 1) {
                fprintf(output, ", ");
            }
        }
        fprintf(output, "}");
    }
    fprintf(output, ";\n");
}

static void generate_array_access(FILE* output, Node* node) {
    fprintf(output, "%s[", node->as.array_access.array_name);
    generate_expression(output, node->as.array_access.index);
    fprintf(output, "]");
}

static void generate_binary_op(FILE* output, Node* node) {
    fprintf(output, "(");
    generate_expression(output, node->as.binary_op.left);
    
    switch (node->as.binary_op.op) {
        case TOKEN_PLUS:
            fprintf(output, " + ");
            break;
        case TOKEN_MINUS:
            fprintf(output, " - ");
            break;
        case TOKEN_STAR:
            fprintf(output, " * ");
            break;
        case TOKEN_SLASH:
            fprintf(output, " / ");
            break;
        case TOKEN_EQUAL_EQUAL:
            fprintf(output, " == ");
            break;
        case TOKEN_NOT_EQUAL:
            fprintf(output, " != ");
            break;
        case TOKEN_LESS:
            fprintf(output, " < ");
            break;
        case TOKEN_LESS_EQUAL:
            fprintf(output, " <= ");
            break;
        case TOKEN_GREATER:
            fprintf(output, " > ");
            break;
        case TOKEN_GREATER_EQUAL:
            fprintf(output, " >= ");
            break;
        default:
            fprintf(stderr, "Unknown binary operator\n");
            break;
    }
    
    generate_expression(output, node->as.binary_op.right);
    fprintf(output, ")");
}

static void generate_expression(FILE* output, Node* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_NUMBER:
            fprintf(output, "%d", node->as.number);
            break;
            
        case NODE_IDENTIFIER:
            fprintf(output, "%s", node->as.string);
            break;
            
        case NODE_ARRAY_ACCESS:
            generate_array_access(output, node);
            break;
            
        case NODE_BINARY_OP:
            generate_binary_op(output, node);
            break;
            
        default:
            fprintf(stderr, "Unknown expression type\n");
            break;
    }
}

static void generate_variable_declaration(FILE* output, Node* node) {
    switch (node->as.var_decl.var_type) {
        case TOKEN_KOPI_O:
            fprintf(output, "    int %s", node->as.var_decl.name);
            break;
        case TOKEN_TEH_O:
            fprintf(output, "    float %s", node->as.var_decl.name);
            break;
        case TOKEN_CHAR_KWAY:
            fprintf(output, "    char %s", node->as.var_decl.name);
            break;
        default:
            fprintf(stderr, "Invalid variable type\n");
            return;
    }
    
    if (node->as.var_decl.initializer) {
        fprintf(output, " = ");
        generate_expression(output, node->as.var_decl.initializer);
    }
    
    fprintf(output, ";\n");
}

static void generate_print(FILE* output, Node* node) {
    fprintf(output, "    printf(%s", node->as.print.format);
    for (int i = 0; i < node->as.print.arg_count; i++) {
        fprintf(output, ", ");
        generate_expression(output, node->as.print.args[i]);
    }
    fprintf(output, ");\n");
}

static void generate_if(FILE* output, Node* node) {
    fprintf(output, "    if (");
    generate_expression(output, node->as.if_stmt.condition);
    fprintf(output, ") {\n");
    
    generate_statement(output, node->as.if_stmt.then_branch);
    
    if (node->as.if_stmt.else_branch) {
        fprintf(output, "    } else {\n");
        generate_statement(output, node->as.if_stmt.else_branch);
    }
    
    fprintf(output, "    }\n");
}

static void generate_loop(FILE* output, Node* node) {
    fprintf(output, "    while (");
    generate_expression(output, node->as.loop.condition);
    fprintf(output, ") {\n");
    
    generate_statement(output, node->as.loop.body);
    
    fprintf(output, "    }\n");
}

static void generate_block(FILE* output, Node* node) {
    for (int i = 0; i < node->as.block.statement_count; i++) {
        generate_statement(output, node->as.block.statements[i]);
    }
}

static void generate_statement(FILE* output, Node* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_VARIABLE_DECL:
            generate_variable_declaration(output, node);
            break;
            
        case NODE_ARRAY_DECL:
            generate_array_declaration(output, node);
            break;
            
        case NODE_PRINT:
            generate_print(output, node);
            break;
            
        case NODE_IF:
            generate_if(output, node);
            break;
            
        case NODE_LOOP:
            generate_loop(output, node);
            break;
            
        case NODE_BLOCK:
            generate_block(output, node);
            break;
            
        case NODE_RETURN:
            fprintf(output, "    return ");
            generate_expression(output, node->as.binary_op.left);
            fprintf(output, ";\n");
            break;
            
        default:
            fprintf(stderr, "Unknown statement type\n");
            break;
    }
}

void generate_code(FILE* output, Node* program) {
    // Generate includes
    fprintf(output, "#include <stdio.h>\n\n");
    
    // Generate main function
    fprintf(output, "int main() {\n");
    
    // Generate program body
    if (program->type == NODE_PROGRAM) {
        generate_block(output, program);
    } else {
        generate_statement(output, program);
    }
    
    // Close main function
    fprintf(output, "    return 0;\n");
    fprintf(output, "}\n");
}
