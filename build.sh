#Clear terminal screen
reset
#Install required libraries
sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo
#Build OS from source
cmake src $*
#Make the object files and the iso
make
make iso