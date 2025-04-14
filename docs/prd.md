# Singlish-C Transpiler PRD

## 1. Product Overview

A transpiler that converts code written in a Singlish-based programming language into standard C code.

## 2. Language Specification

### 2.1 Basic Syntax

- Statement terminator: `lah`
- Comments: Same as C (`//` and `/* */`)

### 2.2 Data Types

- `kopi_o` -> int
- `teh_o` -> float
- `char_kway` -> char

### 2.3 Control Structures

- `can` -> if
- `cannot` -> else
- `loop_until` -> while
- `for_what` -> for

### 2.4 Functions

- Function declaration similar to C but with Singlish types
- `walao` -> return

### 2.5 Basic I/O

- `print_lah` -> printf
- `scan_lah` -> scanf

## 3. Example Programs

### Hello World

```
kopi_o main() {
    print_lah("Hello lah!") lah
    walao 0 lah
}
```

### Simple Calculator

```
kopi_o main() {
    kopi_o x, y lah

    print_lah("First number:") lah
    scan_lah("%d", &x) lah

    print_lah("Second number:") lah
    scan_lah("%d", &y) lah

    print_lah("Sum is: %d", x + y) lah

    walao 0 lah
}
```

## 4. Implementation Phases

### Phase 1: Basic Structure

- Lexical analysis of basic tokens
- Simple parser for Hello World program
- Basic code generation to C

### Phase 2: Core Features

- Full implementation of data types
- Control structures
- Function support

### Phase 3: Standard Library

- Basic I/O functions
- Common utility functions
- Error handling
