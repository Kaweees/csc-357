# Makefile for compiling, linking, and flashing AVR programs to an microcontroller of choice

# Begin Variables Section

## Program Section: change these variables based on your program
# The name of the program to build.
TARGET := uniq

## Compiler Section: change these variables based on your compiler
# -----------------------------------------------------------------------------
# The compiler executable.
CC := gcc
# The compiler flags.
CFLAGS := -g -o $(TARGET) -Wall
# The linker executable.
LD = gcc
# The linker flags.
LDFLAGS = -o $(TARGET) -Wall
# The shell executable.
SHELL := /bin/bash
# The name of the test input file
TEST_INPUT = test_input.txt

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
.PHONY: all $(TARGET) test clean

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
# test:
# 	$(BINS) < $(TEST_INPUT)
# 	@echo "Comparing output to /bin/uniq:"

test: $(BINS)
	@echo "Testing $(TARGET)..."
	@echo "Comparing output to /bin/uniq:"
	diff <($(BINS) < $(TEST_INPUT)) <(uniq < $(TEST_INPUT))

# Clean target: remove build artifacts and non-essential files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

