.PHONY: clean all

CC := gcc
AR := ar

OBJ_DIR := obj
LIB_DIR := ../lib
dummy := $(shell mkdir -p $(OBJ_DIR) $(LIB_DIR))

SRCS = $(wildcard *.c)
OBJS = $(foreach obj, $(patsubst %.c, %.o, $(SRCS)), $(OBJ_DIR)/$(obj))

LIB = $(LIB_DIR)/libbcm2835.a

CFLAGS = -g -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) cr $@ $^

test: $(SRCS)
	$(CC) -DBCM2835_TEST $< -o $@

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $< 

clean:
	rm -rf $(LIB) $(OBJ_DIR) test

