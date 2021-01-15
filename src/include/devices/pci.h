#ifndef PCI_H
#define PCI_H
#include <sys/portio.hpp>
typedef struct
{
    uint16_t vendor, device;
    int driver;
    uint32_t status, command;
    uint16_t bar0, bar1, bar2, bar3, bar4, bar5;
    /* der "bus" und "slot" */
	uint8_t bus;
	uint8_t slot;
	/* pci-to-pci bridge */
	uint8_t start_bus; // SECONDARY
	uint8_t end_bus; // SUBORDINATE
	struct pci_device * parent; // PARENT DEVICE
} pci_device;

typedef struct
{
    pci_device *device;
} pci_device_table;

pci_device* get_discovered_device(uint16_t vendor_id, uint16_t device_id);
void register_pci_device(pci_device* device);
uint16_t pci_config_read_word(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset);
uint16_t pci_check_vendor(uint16_t bus, uint16_t slot);
uint16_t pci_get_vendor(uint8_t bus, uint16_t slot, uint8_t function);
uint16_t pci_check_device(uint16_t bus, uint16_t slot, uint16_t function);
uint16_t pci_get_device(uint16_t bus, uint16_t slot, uint16_t function);
void initialize_pci_devices();
#endif
