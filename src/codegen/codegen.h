#ifndef CODEGEN_H
#define CODEGEN_H

#include "../parser/parser.h"
#include <stdio.h>

void generate_code(FILE* output, Node* program);

#endif // CODEGEN_H 