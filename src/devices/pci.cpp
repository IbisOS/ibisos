#include <devices/pci.h>
#include <stdint.h>
#include <stddef.h>
#include <malloc/malloc.h>
#include <sys/portio.hpp>
#include <graphics/graphics.h>
//A list of all discovered pci devices
pci_device_table discovered_devices[296];
uint32_t discovered_device_count = 0;

pci_device* get_discovered_device(uint16_t vendor_id, uint16_t device_id)
{
    //Loop through the discovered devices
    for(int idx = 0; idx < discovered_device_count; idx++)
    {
        //Get PCI device at idx
        pci_device *device = discovered_devices[idx].device;
        //Get device vendor
        uint16_t device_vendor_id = device->vendor;
        //Check if vendor id match
        if(vendor_id == device_vendor_id)
            //Return the device
            return device;
    }
    
    return NULL;
}

void register_pci_device(pci_device* device)
{
    //Add the device
    discovered_devices[discovered_device_count].device = device;
    //Update device count
    discovered_device_count++;
}

uint16_t pci_config_read_word(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus = (uint32_t) bus;
    uint32_t lslot = (uint32_t) slot;
    uint32_t lfunc = (uint32_t) function;
    uint16_t temp = 0;
    //Create a config address
    address = (uint32_t) ((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset &0xFC) | ((uint32_t) 0x80000000));
    //Write the address
    outl(0xCF8, address);
    //Read the response and return it
    temp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return (temp);
}

uint16_t pci_check_vendor(uint16_t bus, uint16_t slot)
{
    //Read the vendor data from the device
    return pci_config_read_word(bus, slot, 0, 0);
}

uint16_t pci_get_vendor(uint8_t bus, uint16_t slot, uint8_t function)
{
    //Read vendor data with the given function
    return pci_config_read_word(bus, slot, function, 0);
}

uint16_t pci_check_device(uint16_t bus, uint16_t slot, uint16_t function)
{
    //Read the device data from the device
    return pci_config_read_word(bus, slot, function, 2);
}

uint16_t pci_get_device(uint16_t bus, uint16_t slot, uint16_t function)
{
    //Return the found device
    return pci_check_device(bus, slot, function);
}

uint16_t pci_get_bar0(uint16_t bus, uint16_t slot)
{
    //Read BAR0
    return pci_config_read_word(bus, slot, 1, BAR0);
}

uint16_t pci_get_bar1(uint16_t bus, uint16_t slot)
{
    //Read BAR1
    return pci_config_read_word(bus, slot, 1, BAR1);
}

uint16_t pci_get_bar2(uint16_t bus, uint16_t slot)
{
    //Read BAR2
    return pci_config_read_word(bus, slot, 1, BAR2);
}

uint16_t pci_get_bar3(uint16_t bus, uint16_t slot)
{
    //Read BAR3
    return pci_config_read_word(bus, slot, 1, BAR3);
}

uint16_t pci_get_bar4(uint16_t bus, uint16_t slot)
{
    //Read BAR4
    return pci_config_read_word(bus, slot, 1, BAR4);
}

uint16_t pci_get_bar5(uint16_t bus, uint16_t slot)
{
    //Read BAR5
    return pci_config_read_word(bus, slot, 1, BAR5);
}


void initialize_pci_devices()
{
    //Loop through all 256 potential busses
    for(uint32_t bus = 0; bus < 256; bus++)
    {
        //Create a pci device
        pci_device* pci_connected_device = (pci_device*) malloc(sizeof(pci_device));
        //Loop through 32 slots
        for(uint32_t slot = 0; slot < 32; slot++)
        {
            //Get bar0
            uint16_t bar0 = pci_get_bar0(bus, slot);
            //Get bar1
            uint16_t bar1 = pci_get_bar1(bus, slot);
            //Get bar2
            uint16_t bar2 = pci_get_bar2(bus, slot);
            //Get bar3
            uint16_t bar3 = pci_get_bar3(bus, slot);
            //Get uint16_t
            uint16_t bar4 = pci_get_bar4(bus, slot);
            //Get bar5
            uint16_t bar5 = pci_get_bar5(bus, slot);
            //Loop through 8 functions
            for(uint32_t function = 0; function < 8; function++)
            {
                //Get the vendor of the pci device
                uint16_t vendor = pci_get_vendor(bus, slot, function);
                //Get the current device
                uint16_t device = pci_get_device(bus, slot, function);
                //Check if the vendor, device, or base register addresses are nothing
                if(vendor == 0xFFFF || 
                    device  == 0xFFFF || 
                    bar0 == 0xFFFF ||
                    bar1 == 0xFFFF || 
                    bar2 == 0xFFFF || 
                    bar3 == 0xFFFF || 
                    bar4 == 0xFFFF || 
                    bar5 == 0xFFFF)
                    //Go on
                    continue;
                //Set the vendor
                pci_connected_device->vendor = vendor;
                //Set the device
                pci_connected_device->device = device;
                //Set the driver
                pci_connected_device->driver = 0;
                //Set bar0
                pci_connected_device->bar0 = bar0;
                //Set bar1
                pci_connected_device->bar1 = bar1;
                //Set bar2
                pci_connected_device->bar2 = bar2;
                //Set bar3
                pci_connected_device->bar3 = bar3;
                //Set bar4
                pci_connected_device->bar4 = bar4;
                //Set bar5
                pci_connected_device->bar5 = bar5;
                //Register the device
                register_pci_device(pci_connected_device);
            }
        }
    }
}