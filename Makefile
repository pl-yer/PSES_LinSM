CC = gcc
CFLAGS =
SRC_DIR = src
BUILD_DIR = build
TARGET = LinSMtest

DEBUG = 0
ifeq ($(DEBUG),1)
CFLAGS += -g -ggdb3 -O0 -fno-inline -m64 -O3 -Wall -Wextra -fmax-error=5 -DDEBUG
endif

# List all source files
SRC_FILES := ${TARGET}.c

# Default target
all: $(BUILD_DIR) compile run run_cover

# Rule to make the build directory
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# Rule to compile C source files
compile:
	@echo 'Compiling C file...'
	$(CC) $(CFLAGS) $(SRC_DIR)/$(TARGET).c -o $(BUILD_DIR)/$(TARGET).exe

# Run rule
run:
	@echo 'Running $(TARGET) ...'
	./$(BUILD_DIR)/$(TARGET).exe

# Run coverage rule
run_cover:
	@echo 'Running coverage report ...'
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage -g $(SRC_DIR)/$(TARGET).c -o $(BUILD_DIR)/$(TARGET).exe
	move $(TARGET).gcno $(BUILD_DIR)/$(TARGET).gcno
	./$(BUILD_DIR)/$(TARGET).exe
	move $(TARGET).gcda $(BUILD_DIR)/$(TARGET).gcda
	gcov -o $(BUILD_DIR) $(SRC_DIR)/$(TARGET).c
	move *.gcov $(BUILD_DIR)/
	gcovr --html-details -o $(BUILD_DIR)/coverage_report.html

# Clean rule
clean:
	del /Q $(BUILD_DIR) 

.PHONY: all clean run_cover
