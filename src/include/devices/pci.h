#ifndef PCI_H
#define PCI_H
#include <sys/portio.hpp>
#define BAR0    0x10
#define BAR1    0x14
#define BAR2    0x18
#define BAR3    0x1C
#define BAR4    0x20
#define BAR5    0x24

struct pci_device
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
};

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
uint16_t pci_get_bar0(uint16_t bus, uint16_t slot);
uint16_t pci_get_bar1(uint16_t bus, uint16_t slot);
uint16_t pci_get_bar2(uint16_t bus, uint16_t slot);
uint16_t pci_get_bar3(uint16_t bus, uint16_t slot);
uint16_t pci_get_bar4(uint16_t bus, uint16_t slot);
uint16_t pci_get_bar5(uint16_t bus, uint16_t slot);
void initialize_pci_devices();
#endif
