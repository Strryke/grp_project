#!/bin/bash

# Check if a source file was provided
if [ $# -ne 1 ]; then
    echo "Usage: ./build_and_run.sh <source_file.sg>"
    exit 1
fi

SOURCE_FILE=$1
FILENAME=$(basename "$SOURCE_FILE" .sg)
OUTPUTS_DIR="outputs"
OUTPUT_C="$OUTPUTS_DIR/${FILENAME}.c"
OUTPUT_BIN="$OUTPUTS_DIR/$FILENAME"
COMPILER="$OUTPUTS_DIR/singlish-c"

# Create outputs directory if it doesn't exist
mkdir -p "$OUTPUTS_DIR"

# Check if the file exists
if [ ! -f "$SOURCE_FILE" ]; then
    echo "Error: Source file '$SOURCE_FILE' not found"
    exit 1
fi

# Check if the file has .sg extension
if [[ ! $SOURCE_FILE =~ \.sg$ ]]; then
    echo "Error: Source file must have .sg extension"
    exit 1
fi

# Step 1: Make sure the compiler is built
echo "Building Singlish-C compiler..."
make clean && make

if [ $? -ne 0 ]; then
    echo "Error: Failed to build the compiler"
    exit 1
fi

# Step 2: Run the compiler to generate C code
echo "Transpiling $SOURCE_FILE to C..."
"$COMPILER" "$SOURCE_FILE" -o "$OUTPUT_C"

if [ $? -ne 0 ]; then
    echo "Error: Failed to transpile the code"
    exit 1
fi

# Step 3: Compile the generated C code
echo "Compiling generated C code..."
gcc -Wall -Wextra -o "$OUTPUT_BIN" "$OUTPUT_C"

if [ $? -ne 0 ]; then
    echo "Error: Failed to compile the generated C code"
    exit 1
fi

# Step 4: Run the compiled program
echo "Running the program..."
echo "-------------------"
"$OUTPUT_BIN"

# Note: Generated files are kept in the outputs directory
# To clean up, simply delete the outputs directory: rm -rf outputs 