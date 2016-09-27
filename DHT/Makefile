.PHONY: clean all

CC := gcc

OBJ_DIR := obj
BIN_DIR := ../bin
dummy := $(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

SRCS = $(wildcard *.c)
OBJS = $(foreach obj, $(patsubst %.c, %.o, $(SRCS)), $(OBJ_DIR)/$(obj))

BIN = $(BIN_DIR)/DHT

LIB_DIR := ../lib
BCM_DIR := ../bcm2835

CFLAGS = -g -I$(BCM_DIR)
LDFLAGS = -L$(LIB_DIR)

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) $^ -lbcm2835 -o $@

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN) $(OBJ_DIR)
