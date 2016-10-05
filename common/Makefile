.PHONY: clean all

CC := gcc
AR := ar

OBJ_DIR := obj
LIB_DIR := ../lib
dummy := $(shell mkdir -p $(OBJ_DIR) $(LIB_DIR))

SRCS = $(wildcard *.c)
OBJS = $(foreach obj, $(patsubst %.c, %.o, $(SRCS)), $(OBJ_DIR)/$(obj))

LIB = $(LIB_DIR)/libcommon.a

CFLAGS = -g

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) cr $@ $^

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $< 

clean:
	rm -rf $(LIB) $(OBJ_DIR)

