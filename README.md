# Singlish-C Transpiler

A transpiler that converts Singlish-based programming language to standard C code. Write your programs using familiar Singlish expressions!

## Quick Start

### Build the Transpiler

```bash
make
```

### Write Your First Program

Create a file `hello.sg` with:

```
kopi_o main() {
    print_lah("Hello lah!") lah
    walao 0 lah
}
```

### Compile and Run

```bash
# Transpile to C
./singlish-c hello.sg -o hello.c

# Compile the generated C code
gcc hello.c -o hello

# Run your program
./hello
```

## Project Structure

```
singlish-c/
├── src/
│   ├── common/     # Shared utilities
│   ├── lexer/      # Tokenization
│   ├── parser/     # Syntax analysis
│   ├── codegen/    # C code generation
│   └── main.c
├── include/        # Header files
└── examples/       # Example programs
```

## Language Features

See [docs/prd.md](docs/prd.md) for complete language specification.

Basic syntax:

- Use `lah` as statement terminator
- `kopi_o` for integers
- `teh_o` for floats
- `char_kway` for characters
- `can`/`cannot` for if/else
- `print_lah` for output

## Example Programs

Check the `examples/` directory for sample programs, including:

- Hello World
- Simple Calculator
- Basic Control Structures
