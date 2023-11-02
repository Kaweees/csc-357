# Makefile for compiling, linking, and building the program.
# Begin Variables Section

## Program Section: change these variables based on your program
# The name of the program to build.
HENCODE_TARGET := hencode
HDECODE_TARGET := hdecode

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
# The name of the test output files
HENCODE_OUTPUT := hencode_output.txt
HDECODE_OUTPUT := hdecode_output.txt
# The name of the reference executable
REF_EXE := ~pn-cs357/demos/htable
# The name of the reference output file
REF_ENCODE_OUTPUT := expected_output.txt
REF_DECODE_OUTPUT := expected_output.txt

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
HENCODE_SRCS := $(filter-out $(SRC_DIR)/$(HDECODE_TARGET).c,$(wildcard $(SRC_DIR)/*.c))
HDECODE_SRCS := $(filter-out $(SRC_DIR)/$(HENCODE_TARGET).c,$(wildcard $(SRC_DIR)/*.c))
# object files to link
HENCODE_OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(HENCODE_SRCS))
HDECODE_OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(HDECODE_SRCS))
# executable file to build
HENCODE_BIN := $(BUILD_DIR)hencode
HDECODE_BIN := $(BUILD_DIR)hdecode

## Command Section: change these variables based on your commands
# -----------------------------------------------------------------------------
# Targets
.PHONY: all $(HENCODE_TARGET) $(HDECODE_TARGET) test clean debug help

# Default target: build the program
all: $(HENCODE_TARGET) $(HDECODE_TARGET)

# Rule to build hencode
$(HENCODE_TARGET): $(HENCODE_OBJS)
	@mkdir -p $(BUILD_DIR) # Create the build directory if it doesn't exist
	$(LD) $(LDFLAGS) $(HENCODE_OBJS) -o $(HENCODE_BIN)

# Rule to build hdecode
$(HDECODE_TARGET): $(HDECODE_OBJS)
	@mkdir -p $(BUILD_DIR) # Create the build directory if it doesn't exist
	$(LD) $(LDFLAGS) $(HDECODE_OBJS) -o $(HDECODE_BIN)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR) # Create the object directory if it doesn't exist
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

# Test target: build and test the program against sample input
test: $(HENCODE_TARGET) $(HDECODE_TARGET)
	@echo "Testing $(TARGET)..."
	@echo "Testing memory leaks..."
	$(MEMCHECK) $(MEMCHECK_FLAGS) $(HENCODE_BIN) $(TEST_INPUT) $(HENCODE_OUTPUT)
	$(MEMCHECK) $(MEMCHECK_FLAGS) $(HDECODE_BIN) $(HENCODE_OUTPUT) $(HDECODE_OUTPUT)
	@echo "Comparing output to $(REF_EXE):"
	$(REF_EXE) $(TEST_INPUT) $(REF_ENCODE_OUTPUT)
	diff $(HENCODE_OUTPUT) $(REF_ENCODE_OUTPUT)
	$(REF_EXE) $(HENCODE_OUTPUT) $(REF_DECODE_OUTPUT)
	diff $(HDECODE_OUTPUT) $(REF_DECODE_OUTPUT)

# Clean target: remove build artifacts and non-essential files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Debug target: use a debugger to debug the program
debug: $(BINS)
	@mkdir -p $(BUILD_DIR) # Create the build directory if it doesn't exist
	@mkdir -p $(OBJ_DIR) # Create the object directory if it doesn't exist
	@mkdir -p $(BUILD_DIR) # Create the build directory if it doesn't exist
	$(CC) $(CFLAGS) -g $(INCS) -c $(HENCODE_SRCS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(HENCODE_BIN) $(HENCODE_OBJS)
	$(CC) $(CFLAGS) -g $(INCS) -c $(HDECODE_SRCS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(HDECODE_BIN) $(HDECODE_OBJS)
	@echo "Debugging $(TARGET)..."
	$(DEBUGGER) $(DEBUGGER_FLAGS) $(HENCODE_BIN) $(TEST_INPUT) $(HENCODE_OUTPUT)
	$(DEBUGGER) $(DEBUGGER_FLAGS) $(HDECODE_BIN) $(HENCODE_OUTPUT) $(HDECODE_OUTPUT)

# Help target: display usage information
help:
	@echo "Usage: make <target>"
	@echo ""
	@echo "Targets:"
	@echo "  all              Build $(HENCODE_TARGET) and $(HDECODE_TARGET)"
	@echo "  $(HENCODE_TARGET) 	   Build $(HENCODE_TARGET)"
	@echo "  $(HDECODE_TARGET) 	   Build $(HDECODE_TARGET)"
	@echo "  test             Build and test $(HENCODE_TARGET) and $(HDECODE_TARGET) against a sample input, use$(MEMCHECK) to check for memory leaks, and compare the output to $(REF_EXE)"
	@echo "  clean            Remove build artifacts and non-essential files"
	@echo "  debug            Use $(DEBUGGER) to debug $(HENCODE_TARGET) and $(HDECODE_TARGET)"
	@echo "  help             Display this help information"
