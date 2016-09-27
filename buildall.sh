#!/bin/bash

set -e
TARGET=$1
make -C bcm2835 $TARGET
make -C DHT $TARGET
[[ "$TARGET" == "clean" ]] && {
	cd $(dirname $0)
	rm -rf lib bin
}
exit 0
