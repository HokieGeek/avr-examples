#!/bin/sh

bin=${1}
chip=atmega328p

echo "Chip: ${chip}"
echo "Binary: ${bin}"

avrdude -p ${chip} -P usb -c usbtiny -U flash:r:${bin}:i
