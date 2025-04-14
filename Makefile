CC = gcc
CFLAGS = -Wall -Wextra -I./include
SRCDIR = src
BUILDDIR = build
OUTPUTS_DIR = outputs
TARGET = $(OUTPUTS_DIR)/singlish-c

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c) \
          $(wildcard $(SRCDIR)/common/*.c) \
          $(wildcard $(SRCDIR)/lexer/*.c) \
          $(wildcard $(SRCDIR)/parser/*.c) \
          $(wildcard $(SRCDIR)/codegen/*.c)

# Object files
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

# Make sure the build directory exists
$(shell mkdir -p $(sort $(dir $(OBJECTS))) $(OUTPUTS_DIR))

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Compile
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILDDIR) $(OUTPUTS_DIR)

.PHONY: all clean 