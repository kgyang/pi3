.PHONY: clean all

CC = gcc
AR = ar

LIB_SRC_FILES = bcm2835.c
LIB_OBJ_FILES = $(notdir $(patsubst %.c, %.o, $(LIB_SRC_FILES)))

BCM2835_LIB = libbcm2835

CFLAGS = -g

all: $(BCM2835_LIB)

$(BCM2835_LIB): $(LIB_OBJ_FILES)
	$(AR) cr $@ $^

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(LIB_OBJ_FILES) $(BCM2835_LIB)

