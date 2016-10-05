.PHONY: clean all

CC := gcc

OBJ_DIR := obj
BIN_DIR := ../bin
dummy := $(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

SRCS = $(wildcard *.c)
BINS = $(foreach bin, $(patsubst %.c, %, $(SRCS)), $(BIN_DIR)/$(bin))

LIB_DIR := ../lib
BCM_DIR := ../bcm2835

CFLAGS = -g -I$(BCM_DIR)
LDFLAGS = -L$(LIB_DIR)

all: $(BINS)

$(BIN_DIR)/% : $(OBJ_DIR)/%.o
	$(CC) $(LDFLAGS) $^ -lbcm2835 -o $@

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BINS) $(OBJ_DIR)
