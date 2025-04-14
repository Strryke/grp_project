# Singlish-C Compiler

A compiler for the Singlish-C programming language, which transpiles Singlish-C code to C.

## Quick Start

### Prerequisites

- GCC compiler
- Make

### Building

```bash
# Build the compiler
make

# Clean build artifacts
make clean
```

### Running Programs

1. Write your Singlish-C program (e.g., `hello.sg`)
2. Use the build script:

```bash
./build_and_run.sh hello.sg
```

## Project Structure

```
.
├── build_and_run.sh    # Script to build and run programs
├── Makefile           # Build configuration
├── src/               # Source code
│   ├── lexer/        # Lexical analysis
│   ├── parser/       # Syntax analysis
│   ├── codegen/      # Code generation
│   └── common/       # Common utilities
├── include/          # Header files
├── examples/         # Example programs
├── outputs/          # Generated files
└── docs/            # Documentation
    └── language.md   # Language specification
```

## Documentation

- [Language Specification](docs/language.md) - Detailed language features and syntax
- Example programs in `examples/` directory

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

MIT License
