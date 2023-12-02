# Makefile for compiling, linking, and building the program.
# Begin Variables Section

## Program Section: change these variables based on your program
# The name of the program to build.
TARGET := mytalk

## Handin Section: change these variables based on your handin
# -----------------------------------------------------------------------------
# The assignment number.
HANDIN_ASGN := asgn5
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
LDFLAGS :=
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
# The arguments to pass to the test input
TEST_ARGS :=  vcf
# The name of the test output file
TEST_OUTPUT := test_output.tar
# The name of the reference executable
REF_EXE := ~pn-cs357/demos/mytar
# The name of the reference output file
REF_OUTPUT := ref_output.tar

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
BUILD_DIR := $(TOP_DIR)/

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
.PHONY: all $(TARGET) test clean debug help handin latedays testcases

# Default target: build the program
all: $(BINS)

# Build the program
$(TARGET): $(BINS)

# Rule to build the program from linked object files
$(BINS): $(OBJS)
	@mkdir -p $(BUILD_DIR) # Create the build directory if it doesn't exist
	$(LD) $(LDFLAGS) $(OBJS) -L /home/pn-cs357/Given/Talk/lib64 -ltalk -lncurses -o $(BINS)

# Rule to compile source files into object files 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR) # Create the object directory if it doesn't exist
	$(CC) $(CFLAGS) $(INCS) -I ~pn-cs357/Given/Talk/include -c $< -o $@

# Test target: build and test the program against sample input
test: $(TARGET)
	@echo "Testing $(TARGET)..."
	$(BINS) $(TEST_ARGS) $(TEST_OUTPUT) $(TEST_INPUT)
	$(REF_EXE) $(TEST_ARGS)  $(REF_OUTPUT) $(TEST_INPUT)
	@echo "Comparing output to $(REF_EXE):"
	diff <(xxd $(REF_OUTPUT)) <(xxd $(TEST_OUTPUT))

# @echo "Testing memory leaks..."
# $(MEMCHECK) $(MEMCHECK_FLAGS) $(BINS)
# @echo "Comparing output to $(REF_EXE):"
# diff <($(BINS)) <($(REF_EXE))

# Clean target: remove build artifacts and non-essential files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Debug target: use a debugger to debug the program
debug: $(BINS)	
	@echo "Debugging $(TARGET)..."
	$(DEBUGGER) $(DEBUGGER_FLAGS) $(BINS)

# Help target: display usage information
help:
	@echo "Usage: make <target>"
	@echo ""
	@echo "Targets:"
	@echo "  all              Build $(TARGET)"
	@echo "  $(TARGET) 	   Build $(TARGET)"
	@echo "  test             Build and test $(TARGET) against a sample input, use $(MEMCHECK) to check for memory leaks, and compare the output to $(REF_EXE)"
	@echo "  clean            Remove build artifacts and non-essential files"
	@echo "  debug            Use $(DEBUGGER) to debug $(HENCODE_TARGET) and $(HDECODE_TARGET)"
	@echo "  help             Display this help information"
	@echo "  handin           Hand in the assignment using the handin program"
	@echo "  latedays         Reopen the assignment using the latedays program"
	@echo "  testcases        Run the program against the testcases"

# Handin target: hand in the assignment using the handin program
handin:
	@echo "Submitting $(HANDIN_ASGN) via handin..."
	handin $(HANDIN_CLASS) $(HANDIN_ASGN) *.c *.h Makefile README

# Latedays target: reopen the assignment using the latedays program
latedays:
	@echo "Reopening $(HANDIN_ASGN) via latedays..."

# Testcases target: run the program against the testcases
testcases:
	cp include/*.h ./
	cp src/*.c ./
	@echo "Running testcases..."
	~pn-cs357/demos/try$(shell L1=$(HANDIN_ASGN); echo $${L1^})
	rm *.c *.h