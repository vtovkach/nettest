CC      := gcc
CFLAGS  := -Wall -Wextra

ifdef DEBUG
	CFLAGS += -DDEBUG -g -O0
else
	CFLAGS += -O2
endif

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

TARGET  := $(BIN_DIR)/testnet

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $@ -lyaml

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) _net_test

test:
	$(MAKE) -C test run

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	$(MAKE) -C test clean

.PHONY: all run test clean