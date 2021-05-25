#!/bin/bash
rm -rf CMakeCache.txt CMakeFiles
export OUTPUT_DIR=`pwd`/output

#Show a list of all devices
echo "List of all devices that your instance of IbisOS supports:"
#Loop through all supported devices
for device in "supported_devices"/*
do
    #Echo the name of this device
    echo $device | sed -e "s/supported_devices\///g"
done

#Ask the user to select a device
read -p "Device to build (i.e. i686-pc-none-elf): " selected_device

#Did the user select a device?
if [ -z $selected_device ]
then
    selected_device="i686-pc-none-elf"
fi

#Ask for the architecture the user is targeting
read -p "Target architecture (i.e. i686): " selected_arch

#Check if selected architecture is empty
if [ -z $selected_arch ]
then
    selected_arch="i686"
fi

mkdir -p "src/boot"
cp "libs/bootstrap/$selected_arch-boot.asm" "src/boot/boot.asm"
cp "libs/bootstrap/$selected_arch-linker.ld" "src/boot/linker.ld"

export TARGET_BUILD=$selected_device
export TARGET_ARCH=$selected_arch
export TARGET_OUTPUT="output/$TARGET_BUILD"
mkdir -p output

cmake `pwd`/src
make

mkdir -p "$TARGET_OUTPUT/isodir/boot/grub"
cp "libs/bootstrap/grub.cfg" "$TARGET_OUTPUT/isodir/boot/grub/grub.cfg"
mv "$TARGET_OUTPUT/IbisOS.bin" "$TARGET_OUTPUT/isodir/boot/IbisOS.bin"
grub-mkrescue "$TARGET_OUTPUT/isodir" -o "output/$TARGET_BUILD-IbisOS.iso"
rm -rf "src/$selected_device"