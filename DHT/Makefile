.PHONY: clean all

CC = gcc

OBJ_DIR = obj
BIN_DIR = bin

vpath $(BIN_DIR)
vpath $(OBJ_DIR)

SRC_FILES = DHT.c
OBJ_FILES = $(notdir $(patsubst %.c, %.o, $(SRC_FILES)))

BIN = DHT

CFLAGS = -g -I../bcm2835/
LDFLAGS = -static -L../bcm2835/lib/ -lbcm2835

all: $(BIN)

$(BIN): $(OBJ_FILES)
	mkdir -p $(BIN_DIR);\
	$(CC) $(LDFLAGS) -o $(BIN_DIR)/$@ $(OBJ_DIR)/$^

%.o: %.c
	mkdir -p $(OBJ_DIR);\
	$(CC) $(CFLAGS) -c $< -o $(OBJ_DIR)/$@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
