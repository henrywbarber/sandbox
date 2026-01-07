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
MAIN_SRC = $(SRC_DIR)/main.c
TEST_SRC = $(SRC_DIR)/test_hash_table.c

# Object files
HASH_TABLE_OBJ = $(BUILD_DIR)/hash_table.o
PRIME_OBJ = $(BUILD_DIR)/prime.o
MAIN_OBJ = $(BUILD_DIR)/main.o
TEST_OBJ = $(BUILD_DIR)/test_hash_table.o

# Executables
MAIN_EXEC = $(BUILD_DIR)/main
TEST_EXEC = $(BUILD_DIR)/test_ht

# Default target
.PHONY: all
all: build

# Build main executable
.PHONY: build
build: $(MAIN_EXEC)

$(MAIN_EXEC): $(MAIN_OBJ) $(HASH_TABLE_OBJ) $(PRIME_OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Built main executable: $(MAIN_EXEC)"

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

# Run main program
.PHONY: run
run: $(MAIN_EXEC)
	@echo "Running main program..."
	@./$(MAIN_EXEC)

# Build and run tests
.PHONY: test
test: $(TEST_EXEC)
	@echo "Running tests..."
	@./$(TEST_EXEC)

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/main $(BUILD_DIR)/test_ht
	@echo "Clean complete"

# Clean everything including build directory
.PHONY: distclean
distclean:
	@echo "Removing build directory..."
	rm -rf $(BUILD_DIR)
	@echo "Distclean complete"

# Show help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  make build     - Build the main executable (default)"
	@echo "  make run       - Build and run the main program"
	@echo "  make test      - Build and run tests"
	@echo "  make clean     - Remove compiled files"
	@echo "  make distclean - Remove entire build directory"
	@echo "  make help      - Show this help message"