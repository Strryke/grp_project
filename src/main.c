#include "singlish_c.h"

void print_usage(const char* program_name) {
    printf("Usage: %s input_file [-o output_file]\n", program_name);
    printf("Options:\n");
    printf("  -o <file>    Specify output file (default: input_file.c)\n");
    printf("  -h           Show this help message\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char* input_file = NULL;
    const char* output_file = NULL;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) {
                output_file = argv[++i];
            } else {
                fprintf(stderr, "Error: -o requires an output file\n");
                return 1;
            }
        } else if (input_file == NULL) {
            input_file = argv[i];
        } else {
            fprintf(stderr, "Error: Unexpected argument '%s'\n", argv[i]);
            return 1;
        }
    }

    // If no output file specified, create one based on input file
    char default_output[1024];
    if (output_file == NULL) {
        strncpy(default_output, input_file, sizeof(default_output) - 3);
        char* dot = strrchr(default_output, '.');
        if (dot != NULL) {
            *dot = '\0';
        }
        strncat(default_output, ".c", sizeof(default_output) - strlen(default_output) - 1);
        output_file = default_output;
    }

    // Compile the file
    ErrorCode result = compile_file(input_file, output_file);
    if (result != SUCCESS) {
        fprintf(stderr, "Compilation failed with error code: %d\n", result);
        return 1;
    }

    printf("Successfully compiled %s to %s\n", input_file, output_file);
    return 0;
} 