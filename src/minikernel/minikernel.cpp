
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
    //Create a simple device
    struct vfs_device *device = create_device("Rocketeer");
    //Create a folder
    device->create_folder("/", (uint16_t *) 0x000000, device);
    device->create_folder("/test", (uint16_t *) 0x100, device);
    //Get root folder
    struct vfs_folder_node* folder = device->find_folder_node("/", device);
    //Check if folder is not null
    if(folder != NULL)
    {
        //Draw a white circle
        draw_circle((uint32_t*) multiboot_struct[22], 100, 100, 100, 0xFFFFFF);
    }
}