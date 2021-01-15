
#include <devices/pci.h>
#include <malloc/malloc.h>
#include <system.h>
#include <vesa/vesa.h>
#include <graphics/graphics.h>
#include <sys/portio.hpp>
#include <storage/ata.h>
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
    //Initialize ata
    ata_drive* drive = initialize_ata_driver();
    drive->write((uint32_t *) 0x282828, drive);
    draw_circle((uint32_t *) multiboot_struct[22], 100, 100, 100, 0xFFFFFF);
}