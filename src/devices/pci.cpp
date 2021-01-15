#include <devices/pci.h>
#include <stdint.h>
#include <stddef.h>
#include <malloc/malloc.h>
#include <sys/portio.hpp>
#include <graphics/graphics.h>
//A list of all discovered pci devices
pci_device_table discovered_devices[296];
uint32_t discovered_device_count = 0;

// Modified so the enumeration is less brute force and more topomap centric
// This way, the kernel can use PCI to PCI bridges mroe flexibly instead of scanning troughout all of 'em

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


void recognize_device(pci_device * parent, uint8_t bus, uint8_t slot) {
	if(pci_get_vendor(bus,slot,0) == 0xFFFF || pci_get_device(bus,slot,0) == 0xFFFF) {
		return;
	}
	uint16_t header = (pci_config_read_word(bus,slot,0,0x0C)>>0xFFFF)&0xFF;
	
	//Create a pci device
    pci_device* current = (pci_device*) malloc(sizeof(pci_device));
	current->parent = parent;
	
	// Set stuff 
	current->vendor = pci_get_vendor(bus,slot,0);
	current->device = pci_get_device(bus,slot,0);
	current->driver = 0;
    current->bar0 = pci_get_bar0(bus, slot);
    current->bar1 = pci_get_bar1(bus, slot);
    current->bar2 = pci_get_bar2(bus, slot);
    current->bar3 = pci_get_bar3(bus, slot);
    current->bar4 = pci_get_bar4(bus, slot);
	current->bar5 = pci_get_bar5(bus, slot);

	// Needed
	current->bus = bus;
	current->slot = slot;
	
	// Check for bridges
	if(header != 0x00) {
		uint8_t sec_bus = (pci_config_read_word(bus,slot,0,0x18)>>0xFF)&0xFF;
		uint8_t sub_bus = (pci_config_read_word(bus,slot,0,0x18)>>0xFFFF)&0xFF;
		for(int i = sec_bus; i < sub_bus; i++) {
			scan_bus(parent,i);
		}
	}
	
	// Register into list
	register_pci_device(current);
	return;
}

void scan_bus(pci_device * parent, uint8_t bus) {
	for(int dev = 0; dev < 32; dev++) {
		recognize_device(parent,bus,dev);
	}
	return;
}

void initialize_pci_devices() {
	scan_bus(NULL,0);
	return;
}