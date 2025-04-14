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

You have two options to compile and run your Singlish-C programs:

#### Option 1: Using the build script (Recommended)

The easiest way to compile and run your programs is using the `build_and_run.sh` script:

```bash
# Make the script executable
chmod +x build_and_run.sh

# Run your program
./build_and_run.sh examples/hello.sg
```

This will automatically:

1. Build the compiler if needed
2. Transpile your Singlish-C code to C
3. Compile the generated C code
4. Run the resulting program

#### Option 2: Manual Compilation

If you prefer to do it step by step:

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
├── examples/       # Example programs
└── build_and_run.sh # Quick build and run script
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
- `loop_until` for while loops
- `for_what` for for loops

## Example Programs

Check the `examples/` directory for sample programs, including:

- Hello World
- Simple Calculator
- Basic Control Structures
- Loop Examples
