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

# The name of the test executable for FORKIT
TRYIT_TEST_INPUT := /bin/cat
# The flags to pass to the test executable for FORKIT
TRYIT_TEST_INPUT_FLAGS := README

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
.PHONY: all $(FORKIT_TARGET) $(TRYIT_TARGET) memcheck test clean debug handin latedays testcases help

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

# Memcheck target: run the program through a memory checker
memcheck: $(FORKIT_TARGET) $(TRYIT_TARGET)
	@echo "Checking $(FORKIT_TARGET) and $(TRYIT_TARGET) for memory leaks..."
	$(MEMCHECK) $(MEMCHECK_FLAGS) $(FORKIT_BIN)
	$(MEMCHECK) $(MEMCHECK_FLAGS) $(TRYIT_BIN) $(TRYIT_TEST_INPUT) $(TRYIT_TEST_INPUT_FLAGS)

# Test target: build and test the program against sample input
test: $(FORKIT_TARGET) $(TRYIT_TARGET)
	@echo "Testing $(FORKIT_TARGET)..."
	$(FORKIT_BIN)
	@echo "Testing $(TRYIT_TARGET)..."
	$(TRYIT_TEST_INPUT) $(TRYIT_TEST_INPUT_FLAGS)

# Clean target: remove build artifacts and non-essential files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	rm $(FORKIT_OUTPUT) $(TRYIT_OUTPUT)

# Debug target: use a debugger to debug the program
debug: $(BINS)
	@echo "Debugging $(TARGET)..."
	$(DEBUGGER) $(DEBUGGER_FLAGS) $(FORKIT_BIN)
	$(DEBUGGER) $(DEBUGGER_FLAGS) $(TRYIT_BIN) $(TRYIT_TEST_INPUT) $(TRYIT_TEST_INPUT_FLAGS)

# Handin target: hand in the assignment using the handin program
handin:
	cp include/*.h ./
	cp src/*.c ./
	@echo "Submitting $(HANDIN_ASGN) via handin..."
	handin $(HANDIN_CLASS) $(HANDIN_ASGN) *.c *.h Makefile README
	rm *.c *.h

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

# Help target: display usage information
help:
	@echo "Usage: make <target>"
	@echo ""
	@echo "Targets:"
	@echo "  all              Build $(FORKIT_TARGET) and $(TRYIT_TARGET)"
	@echo "  $(FORKIT_TARGET) 	   Build $(FORKIT_TARGET)"
	@echo "  $(TRYIT_TARGET) 	   Build $(TRYIT_TARGET)"
	@echo "  clean            Remove build artifacts and non-essential files"
	@echo "  test             Build and test $(FORKIT_TARGET) and $(TRYIT_TARGET) against a sample input, and compare the output to $(REF_EXE)"
	@echo "  memcheck         Run $(MEMCHECK) on $(FORKIT_TARGET) and $(TRYIT_TARGET) to check for memory leaks"
	@echo "  debug            Use $(DEBUGGER) to debug $(FORKIT_TARGET) and $(TRYIT_TARGET)"
	@echo "  handin           Hand in the assignment using the handin program"
	@echo "  latedays         Reopen the assignment using the latedays program"
	@echo "  testcases        Run $(FORKIT_TARGET) and $(TRYIT_TARGET) against the testcases"
	@echo "  help             Display this help information"
