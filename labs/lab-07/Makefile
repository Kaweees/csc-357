# Makefile for compiling, linking, and building the program.
# Begin Variables Section

## Program Section: change these variables based on your program
# The name of the program to build.
FORKIT_TARGET := forkit
TRYIT_TARGET := tryit

## Handin Section: change these variables based on your handin
# -----------------------------------------------------------------------------
# The assignment number.
HANDIN_ASGN := asgn3
# The class name.
HANDIN_CLASS := pn-cs357

## Compiler Section: change these variables based on your compiler
# -----------------------------------------------------------------------------
# The compiler executable.
CC := gcc
# The compiler flags.
CFLAGS := -Wall -g
# The linker executable.
LD := gcc
# The linker flags.
LDFLAGS := -Wall -g
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
TEST_INPUT := tests/12_longcodes
# The name of the test output files
FORKIT_OUTPUT := FORKIT_output.huff
TRYIT_OUTPUT := TRYIT_output
# The name of the reference executable
REF_EXE := ~pn-cs357/demos/htable

# The name of the reference output file
REF_ENCODE_OUTPUT := tests/12_longcodes.huff
REF_TRYIT_OUTPUT := tests/12_longcodes

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
FORKIT_SRCS := $(filter-out $(SRC_DIR)/$(TRYIT_TARGET).c,$(wildcard $(SRC_DIR)/*.c))
TRYIT_SRCS := $(filter-out $(SRC_DIR)/$(FORKIT_TARGET).c,$(wildcard $(SRC_DIR)/*.c))
# object files to link
FORKIT_OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(FORKIT_SRCS))
TRYIT_OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(TRYIT_SRCS))
# executable file to build
FORKIT_BIN := $(BUILD_DIR)$(FORKIT_TARGET)
TRYIT_BIN := $(BUILD_DIR)$(TRYIT_TARGET)

## Command Section: change these variables based on your commands
# -----------------------------------------------------------------------------
# Targets
.PHONY: all $(FORKIT_TARGET) $(TRYIT_TARGET) test clean debug help

# Default target: build the program
all: $(FORKIT_TARGET) $(TRYIT_TARGET)

# Rule to build FORKIT
$(FORKIT_TARGET): $(FORKIT_OBJS)
	@mkdir -p $(BUILD_DIR) # Create the build directory if it doesn't exist
	$(LD) $(LDFLAGS) $(FORKIT_OBJS) -o $(FORKIT_BIN)

# Rule to build TRYIT
$(TRYIT_TARGET): $(TRYIT_OBJS)
	@mkdir -p $(BUILD_DIR) # Create the build directory if it doesn't exist
	$(LD) $(LDFLAGS) $(TRYIT_OBJS) -o $(TRYIT_BIN)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR) # Create the object directory if it doesn't exist
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

# Test target: build and test the program against sample input
test: $(FORKIT_TARGET) $(TRYIT_TARGET)
	@echo "Testing $(FORKIT_TARGET) and $(TRYIT_TARGET)..."
	@echo "Testing memory leaks..."
	$(MEMCHECK) $(MEMCHECK_FLAGS) $(FORKIT_BIN) $(TEST_INPUT) $(FORKIT_OUTPUT)
	$(MEMCHECK) $(MEMCHECK_FLAGS) $(TRYIT_BIN) $(FORKIT_OUTPUT) $(TRYIT_OUTPUT)
	@echo "Comparing output to $(REF_EXE):"
	xxd <(diff $(FORKIT_OUTPUT) $(REF_ENCODE_OUTPUT))
	diff $(TRYIT_OUTPUT) $(REF_TRYIT_OUTPUT)

# Clean target: remove build artifacts and non-essential files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	rm $(FORKIT_OUTPUT) $(TRYIT_OUTPUT)

# Debug target: use a debugger to debug the program
debug: $(BINS)
	
	@echo "Debugging $(TARGET)..."
	$(DEBUGGER) $(DEBUGGER_FLAGS) $(FORKIT_BIN) $(TEST_INPUT) $(FORKIT_OUTPUT)
	$(DEBUGGER) $(DEBUGGER_FLAGS) $(TRYIT_BIN) $(FORKIT_OUTPUT) $(TRYIT_OUTPUT)

# Help target: display usage information
help:
	@echo "Usage: make <target>"
	@echo ""
	@echo "Targets:"
	@echo "  all              Build $(FORKIT_TARGET) and $(TRYIT_TARGET)"
	@echo "  $(FORKIT_TARGET) 	   Build $(FORKIT_TARGET)"
	@echo "  $(TRYIT_TARGET) 	   Build $(TRYIT_TARGET)"
	@echo "  test             Build and test $(FORKIT_TARGET) and $(TRYIT_TARGET) against a sample input, use$(MEMCHECK) to check for memory leaks, and compare the output to $(REF_EXE)"
	@echo "  clean            Remove build artifacts and non-essential files"
	@echo "  debug            Use $(DEBUGGER) to debug $(FORKIT_TARGET) and $(TRYIT_TARGET)"
	@echo "  help             Display this help information"
	@echo "  handin           Hand in the assignment using the handin program"
	@echo "  latedays         Reopen the assignment using the latedays program"

# Handin target: hand in the assignment using the handin program
handin:
	@echo "Submitting $(HANDIN_ASGN) via handin..."
	handin $(HANDIN_CLASS) $(HANDIN_ASGN) *.c *.h Makefile README

# Latedays target: reopen the assignment using the latedays program
latedays:
	@echo "Reopening $(HANDIN_ASGN) via latedays..."