#include <devices/pci.h>
#include <stdint.h>
#include <stddef.h>
#include <malloc/malloc.h>
#include <sys/portio.hpp>
//A list of all discovered pci devices
pci_device_table discovered_devices[256];
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

static void scan_bus(pci_device * parent, uint8_t bus) {
	for(int dev = 0; dev < 32; dev++) {
		recognize_device(parent,bus,dev);
	}
	return;
}

static void recognize_device(pci_device * parent, uint8_t bus, uint8_t slot) {
	if(pci_check_vendor(bus,slot,0) == 0xFFFF || pci_check_device(bus,slot,0) == 0xFFFF) {
		return;
	}
	uint16_t header = (pci_config_read_word(bus,slot,0,0x0C)>>0xFFFF)&0xFF;
	
	//Create a pci device
        pci_device* current = (pci_device*) malloc(sizeof(pci_device));
	current->parent = parent;
	
	// Set stuff 
	current->vendor = pci_check_vendor(bus,slot,0);
	current->device = pci_check_device(bus,slot,0);
	current->driver = 0;
	
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

void initialize_pci_devices() {
	scan_bus(NULL,0);
	return;
}
