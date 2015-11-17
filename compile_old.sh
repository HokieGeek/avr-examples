#!/bin/sh

src=$1
chip=`echo ${src} | cut -d'/' -f1 | tr '[:upper:]' '[:lower:]'`
out=`echo ${src} | cut -d'.' -f1`".hex"

echo "Chip: ${chip}"

compiler_flags="-Wall -Os \
		-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums \
		-Wstrict-prototypes -Wa,-adhlns=./main.lst" # -MP -MF"
#compiler_flags="-gdwarf-2 -Wall -Os \
#		-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums \
#		-Wstrict-prototypes -Wa,-adhlns=./main.lst -MMD -MP -MF"

#avr-gcc -c -I. -DF_CPU=1200000UL -Wa,-adhlns=./main.lst
#avr-gcc -c -mmcu=attiny13 -I. -gdwarf-2 -DF_CPU=1200000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes -Wa,-adhlns=./main.lst  -std=gnu99 -MMD -MP -MF .dep/main.o.d main.c -o main.o

set -x
avr-gcc -std=gnu99 ${compiler_flags} -mmcu=${chip} -o ${out} ${src}
#avr-gcc -I. -std=c99 -mmcu=${chip} -o ${out} ${src}
