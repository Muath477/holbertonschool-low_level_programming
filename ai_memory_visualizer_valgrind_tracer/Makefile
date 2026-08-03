CC = gcc
CFLAGS = -Wall -Wextra -g
SRC_DIR = programs
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
BINARIES = $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%,$(SOURCES))

all: $(BIN_DIR) $(BINARIES)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/%: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)

re: clean all

.PHONY: all clean re
