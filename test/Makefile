CC := gcc

SRC_DIR := ../src
TEST_DIR := src

BUILD_DIR := build
BIN_DIR := bin
TEST_TARGET := $(BIN_DIR)/test_runner

CFLAGS := -Wall -Wextra -I$(SRC_DIR)

ifdef DEBUG
	CFLAGS += -DDEBUG -g -O0
else 
	CFLAGS += -O2
endif

LDFLAGS := -lyaml

SRCS := $(wildcard $(SRC_DIR)/*.c)
APP_SRCS := $(filter-out $(SRC_DIR)/main.c, $(SRCS))

TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)

APP_OBJS := $(APP_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/src/%.o)
TEST_OBJS := $(TEST_SRCS:$(TEST_DIR)/%.c=$(BUILD_DIR)/test/%.o)

all: $(TEST_TARGET)

run: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(APP_OBJS) $(TEST_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)/src
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(BUILD_DIR)/test
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all run clean