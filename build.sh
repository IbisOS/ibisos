#Clear terminal screen
reset
#Install required libraries
echo "Installing build-essential, bison, flex, libgmp3-dev, libmpc-dev, libmpfr-dev, texinfo, clang, cmake, nasm, xorriso, mtools, qemu-system, and qemu"
sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo clang cmake nasm xorriso mtools qemu-system qemu -y 1&>/dev/null
echo "Done."
#Build OS from source
echo "Setting up environment for building..."
cmake src $* 1&>/dev/null
echo "Done. Building project..."
#Make the object files and the iso
make 1&>/dev/null
echo "Done. Building ISO file..."
make iso
echo "Done."