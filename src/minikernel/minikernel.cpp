
#include <devices/pci.h>
#include <malloc/malloc.h>
#include <system.h>
#include <vesa/vesa.h>
#include <graphics/graphics.h>
#include <sys/portio.hpp>
#include <vfs/dev.h>
extern "C" void initiate_mini_kernel(unsigned int *multiboot_struct);

void initiate_mini_kernel(unsigned int *multiboot_struct)
{
    //Initialize the GDT
    init_gdt();
    //Initialize the IDT
    init_idt();
    //Initialize vesa mode
    init_vesa(multiboot_struct);
    //Initialize all PCI devices
    initialize_pci_devices();
}