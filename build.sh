#Clear terminal screen
reset
if [ ! -f ".setup_done" ]
then
    #Install required libraries
    echo "Installing build-essential, bison, flex, libgmp3-dev, libmpc-dev, libmpfr-dev, texinfo, clang, cmake, nasm, xorriso, mtools, qemu-system, and qemu"
    sudo apt remove  build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo clang cmake nasm xorriso mtools qemu-system qemu -y 1&>/dev/null
    echo "Done. Installing other prerequisites..."
    cd compiler/gcc
    ./contrib/download_prerequisites
    echo "Done."
    echo ''>>.setup_done
fi
#Build OS from source
echo "Setting up environment for building..."
cmake src $*
echo "Done. Building project..."
#Make the object files and the iso
make 1&>/dev/null
echo "Done. Building ISO file..."
make iso 1&>/dev/null
echo "Done."
