# Makefile for compiling, linking, and building the program.
# Begin Variables Section

## Program Section: change these variables based on your program
# The name of the program to build.
TARGET := htable

## Compiler Section: change these variables based on your compiler
# -----------------------------------------------------------------------------
# The compiler executable.
CC := gcc
# The compiler flags.
CFLAGS := -Wall
# The linker executable.
LD := gcc
# The linker flags.
LDFLAGS := -Wall
# The shell executable.
SHELL := /bin/bash

## Testing Suite Section: change these variables based on your testing suite
# -----------------------------------------------------------------------------
# The memory checker executable.
MEMCHECK := valgrind
# The memory checker flags.
MEMCHECK_FLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes
# The debugger executable.
DEBUGGER := gdb
# The debugger flags.
DEBUGGER_FLAGS := 

# The name of the test input file
TEST_INPUT := test_input.txt
# The name of the test output file
TEST_OUTPUT := 
# The name of the reference executable
REF_EXE := ~pn-cs357/demos/htable
# The name of the reference output file
REF_OUTPUT := 

## Output Section: change these variables based on your output
# -----------------------------------------------------------------------------
# top directory of project
TOP_DIR := $(shell pwd)
# directory to locate source files
SRC_DIR := $(TOP_DIR)/src
# directory to locate header files
INC_DIR := $(TOP_DIR)/include
# directory to locate object files
OBJ_DIR := $(TOP_DIR)/obj
# directory to place build artifacts
BUILD_DIR := $(TOP_DIR)/target/release/

# header files to preprocess
INCS := -I$(INC_DIR)
# source files to compile
SRCS := $(wildcard $(SRC_DIR)/*.c)
# object files to link
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
# executable file to build
BINS := $(BUILD_DIR)$(TARGET)

## Command Section: change these variables based on your commands
# -----------------------------------------------------------------------------
# Targets
.PHONY: all $(TARGET) test clean debug

# Default target: build the program
all: $(BINS)

# Build the program
$(TARGET): $(BINS)

# Rule to build the program from linked object files
$(BINS): $(OBJS)
	@mkdir -p $(BUILD_DIR) # Create the build directory if it doesn't exist
	$(LD) $(LDFLAGS) $(OBJS) -o $(BINS)

# Rule to compile source files into object files 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR) # Create the object directory if it doesn't exist
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

# Test target: build and test the program against sample input
test: $(BINS)
	@echo "Testing $(TARGET)..."
	@echo "Testing memory leaks..."
	$(BINS) $(TEST_INPUT)
	$(MEMCHECK) $(MEMCHECK_FLAGS) $(BINS) $(TEST_INPUT)
	@echo "Comparing output to $(REF_EXE):"
	diff <($(BINS) $(TEST_INPUT)) <($(REF_EXE) $(TEST_INPUT))
# $(BINS) hdecode output.txt decoded.txt	

# Clean target: remove build artifacts and non-essential files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Debug target: use a debugger to debug the program
debug: $(BINS)
	@mkdir -p $(OBJ_DIR) # Create the object directory if it doesn't exist
	$(CC) $(CFLAGS) -g $(INCS) -c $< -o $@
	@mkdir -p $(BUILD_DIR) # Create the build directory if it doesn't exist
	$(LD) $(LDFLAGS) $(OBJS) -o $(BINS)
	@echo "Debugging $(TARGET)..."
	$(DEBUGGER) $(BINS)