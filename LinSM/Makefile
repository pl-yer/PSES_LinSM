CC = g++
CFLAGS = --std=c++11 -O3 -Wall -Iinc
TOP_DIR = .
SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build
TARGET = test

# List all source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
CPP_FILES := $(wildcard $(TOP_DIR)/*.cpp)

# Generate object file names
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
OBJ_FILES += $(patsubst $(TOP_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_FILES))

# Default target
all: $(BUILD_DIR) $(TARGET) run

# Rule to make the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	
# Rule to compile C source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo 'Compiling C file...'
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile C++ source files
$(BUILD_DIR)/%.o: $(TOP_DIR)/%.cpp
	@echo 'Compiling C++ file...'
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to make the executable
$(TARGET): $(OBJ_FILES)
	@echo 'Linking ...'
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ_FILES)

# Run rule
run:
	@echo 'Running $(TARGET) ...'
	./$(TARGET).exe

# Clean rule
clean:
	del /Q $(BUILD_DIR) $(TARGET).exe

.PHONY: all clean
