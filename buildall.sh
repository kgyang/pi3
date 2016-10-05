#!/bin/bash

TARGET=$1

cd $(dirname $0)

make -C common && make -C bcm2835 $TARGET && make -C DHT $TARGET

[[ "$TARGET" == "clean" ]] && {
	cd $(dirname $0)
	rm -rf lib bin
}

