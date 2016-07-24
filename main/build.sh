#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/internal.ldf

SCRIPT=$(readlink -f "$0")
EXEPATH=$(dirname "$SCRIPT")
cd $EXEPATH

if [ -z "${CROSS_COMPILE+xxx}" ]; then
case $(uname -p) in
	arm*)
		# Use native arm compiler (no cross prefix)
		CROSS_COMPILE=
		;;
	   *)
		# Use cross compiler
		CROSS_COMPILE="arm-linux-gnueabihf-"
		;;
esac
fi

#
#${CROSS_COMPILE}gcc -O3 -o test_host src/test_host.c src/isprime.c -lm

# Build HOST side application
${CROSS_COMPILE}gcc host.c -o host.elf -I ${EINCS} -L ${ELIBS} -le-hal -le-loader -std=c99

# Build DEVICE side program
e-gcc -O2 -T ${ELDF} eCores.c -o eCores.elf -le-lib -lm -std=c99

