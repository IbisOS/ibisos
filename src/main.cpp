#include <debugger>
using namespace std;

extern "C" void kernelMain()
{
    initializeTerminalScreen(LIGHT_RED, WHITE);
    std::writeString("Welcome to IbisOS v0.0.1\n");
}