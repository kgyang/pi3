.PHONY: clean all

CC = gcc
AR = ar

OBJ_DIR = obj
LIB_DIR = lib

vpath $(LIB_DIR)
vpath $(OBJ_DIR)

LIB_SRC_FILES = bcm2835.c
LIB_OBJ_FILES = $(notdir $(patsubst %.c, %.o, $(LIB_SRC_FILES)))

BCM2835_LIB = libbcm2835.a

CFLAGS = 

all: $(BCM2835_LIB)

$(BCM2835_LIB): $(LIB_OBJ_FILES)
	mkdir -p $(LIB_DIR);\
	cd $(OBJ_DIR);\
	$(AR) crs $@ $^;\
	cp $@ ../$(LIB_DIR)

%.o: %.c
	mkdir -p $(OBJ_DIR);\
	$(CC) -c $(CFLAGS) $< -o $(OBJ_DIR)/$@

clean:
	rm -rf $(LIB_DIR) $(OBJ_DIR)

