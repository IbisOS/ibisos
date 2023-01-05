echo "Compiling IbisOS from scratch..."
./build.sh $*
echo "Done. Building ISO and running it in qemu..."
make qemu
echo "Done."