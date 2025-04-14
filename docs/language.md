# Singlish-C Language Specification

## Language Features

### Basic Syntax

- Use `lah` as statement terminator
- `kopi_o` for integers
- `teh_o` for floats
- `char_kway` for characters
- `can`/`cannot` for if/else
- `print_lah` for output
- `loop_until` for while loops
- `for_what` for for loops

### Data Types

### Basic Types

- `kopi_o` - Integer type
- `teh_o` - Float type
- `char_kway` - Character type

### Arrays

Arrays are fixed-size collections of elements of the same type.

#### Declaration

```c
kopi_o numbers[5] lah        // Array of 5 integers
teh_o prices[10] lah         // Array of 10 floats
char_kway letters[26] lah    // Array of 26 characters
```

#### Initialization

```c
kopi_o numbers[5] = {1, 2, 3, 4, 5} lah
teh_o prices[3] = {1.99, 2.99, 3.99} lah
```

#### Accessing Elements

```c
numbers[0] = 10 lah          // Set first element
kopi_o x = numbers[1] lah    // Get second element
```

#### Array Bounds

- Array indices start at 0
- Accessing elements beyond array bounds is undefined behavior
- Array size must be a constant expression

### Control Structures

#### If-Else

```c
can (x > 0) {
    print_lah("Positive lah!") lah
} cannot {
    print_lah("Not positive lah!") lah
}
```

#### While Loop

```c
kopi_o i = 0 lah
loop_until (i < 5) {
    print_lah("Count: %d\n", i) lah
    i = i + 1 lah
}
```

#### For Loop

```c
for_what (kopi_o i = 0 lah i < 5 lah i = i + 1 lah) {
    print_lah("Count: %d\n", i) lah
}
```

### Functions

```c
kopi_o add(kopi_o a, kopi_o b) {
    walao a + b lah
}

kopi_o main() {
    kopi_o result = add(5, 3) lah
    print_lah("Result: %d\n", result) lah
    walao 0 lah
}
```

### Input/Output

```c
print_lah("Hello lah!") lah                    // Basic print
print_lah("Number: %d\n", 42) lah             // Print with format
print_lah("Float: %f\n", 3.14) lah            // Print float
print_lah("Char: %c\n", 'A') lah              // Print character
```
