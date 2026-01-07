# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lm

# Directories
SRC_DIR = src
BUILD_DIR = build

# Source files
HASH_TABLE_SRC = $(SRC_DIR)/hash_table.c
PRIME_SRC = $(SRC_DIR)/prime.c
TEST_SRC = $(SRC_DIR)/test_hash_table.c

# Object files
HASH_TABLE_OBJ = $(BUILD_DIR)/hash_table.o
PRIME_OBJ = $(BUILD_DIR)/prime.o
TEST_OBJ = $(BUILD_DIR)/test_hash_table.o

# Executables
TEST_EXEC = $(BUILD_DIR)/test_ht

# Default target
.PHONY: all
all: test

# Build test executable
$(TEST_EXEC): $(TEST_OBJ) $(HASH_TABLE_OBJ) $(PRIME_OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Built test executable: $(TEST_EXEC)"

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build and run tests
.PHONY: test
test: $(TEST_EXEC)
	@echo "Running tests..."
	@./$(TEST_EXEC)

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/test_ht
	@echo "Clean complete"

# Show help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  make test      - Build and run tests"
	@echo "  make clean     - Remove compiled files"
	@echo "  make help      - Show this help message"