#!/bin/bash
rm -rf CMakeCache.txt CMakeFiles
export OUTPUT_DIR=`pwd`/output
if [ "$1" == "" ]
then
    export TARGET_BUILD="i686-pc-none-elf"
else
    export TARGET_BUILD="$1"
fi

if [ "$2" == "" ]
then
    export TARGET_ARCH="i686"
else
    export TARGET_ARCH="$1"
fi

if [ "$3" == "" ]
then
    export FONT_ARCH="i386"
else
    export FONT_ARCH="$3"
fi

objcopy -O $TARGET_BUILD -B $FONT_ARCH -I binary assets/font.psf font.o
export TARGET_OUTPUT="output/$TARGET_BUILD"
mkdir -p output
cmake `pwd`/src
make

mkdir -p "$TARGET_OUTPUT/isodir/boot/grub"
cp src/boot/grub.cfg "$TARGET_OUTPUT/isodir/boot/grub/grub.cfg"
mv $TARGET_OUTPUT/IbisOS.bin "$TARGET_OUTPUT/isodir/boot/IbisOS.bin"
grub-mkrescue "$TARGET_OUTPUT/isodir" -o "output/$TARGET_BUILD-IbisOS.iso"
