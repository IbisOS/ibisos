#Check if first argument is empty 
if [ "$1" == "" ]
then
    #Execute this script with x86-64 and x86-64-pc-none-elf as build parameters
    ./build.sh "x86-64" "x86-64-pc-none-elf"
else
    #Create output directories for bootloader, compiler, and os
    mkdir -p output/$2/bootloader
    mkdir -p output/$2/compiler
    mkdir -p output/$2/os

    #Install required programs (nasm and clang)
    echo "Installing required programs..."
    sudo apt install -y nasm clang
    chmod a+x bootloader/build.sh

    #Build everything
    echo "Building IbisBoot..."
    nasm -f bin src/bootloader/$1/bootloader.asm -o output/$2/bootloader/bootloader.bin

    qemu-system-x86_64 output/$2/bootloader/bootloader.bin
fi