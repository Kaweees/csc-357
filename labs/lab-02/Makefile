# Makefile for compiling, linking, and flashing AVR programs to an microcontroller of choice

# Begin Variables Section

## Compiler Section: change these variables based on your compiler
# -----------------------------------------------------------------------------
# The `gcc` executable.
CC := gcc
CFLAGS := -g -o uniq -Wall

## Output Section: change these variables based on your output
# -----------------------------------------------------------------------------
# top directory of project
TOP_DIR := $(shell pwd)
# directory to locate source files
SRC_DIR := $(TOP_DIR)/src
# directory to locate header files
INC_DIR := $(TOP_DIR)/include

# name of target executable
TARGET := uniq
# directory to place build artifacts
BUILD_DIR := $(TOP_DIR)/target/release/

# source files to compile
SRCS := $(wildcard $(SRC_DIR)/*.c)
# header files to include
HDRS := $(wildcard $(INC_DIR)/*.h)
# object files to build
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TARGET_OBJ := $(BUILD_DIR)/uniq.o


## Command Section: change these variables based on your commands
# -----------------------------------------------------------------------------
# Targets
.PHONY: all $(TARGET) test clean

all: clean $(TARGET) test

# $< and $@ are automatic variables that refer to the source (dependency) and target (build) files, respectively
# Use the GCC compiler to compile source files into an object files
$(TARGET): $(TARGET_OBJ)

$(TARGET_OBJ): $(SOURCE)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $(TARGET_OBJ) $(SOURCE) $(HEADER)

# Test the program by running the executable
test:
	./$(TARGET)


# object files to build
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

## Dependency Section: change these variables based on your dependencies
# -----------------------------------------------------------------------------
# This is the default target for `make`.
# The `make` command will invoke the first target entry in the file.
# In this case, the first target is `all`, which is used to compile the program.
# If you want to compile the program, type `make` or `make all` in the terminal.
all: $(TARGET)

# This is the target for `make test`.
# The `make test` command will invoke the `test` target entry in the file.
# In this case, the `test` target is used to run the program.
# If you want to run the program, type `make test` in the terminal.
test: $(TARGET)
	./$(TARGET)

# This is the target for `make clean`.
# The `make clean` command will invoke the `clean` target entry in the file.
# In this case, the `clean` target is used to remove build artifacts.
# If you want to remove build artifacts, type `make clean` in the terminal.
clean:
	rm -rf $(TARGET) $(OBJS)

# This is the target for `make flash`.
# The `make flash` command will invoke the `flash` target entry in the file.
# In this case, the `flash` target is used to flash the program to an Arduino.
# If you want to flash the program to an Arduino, type `make flash` in the terminal.
flash: $(TARGET)
	./$(TARGET)
