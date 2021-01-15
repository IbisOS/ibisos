#ifndef ATA_H
#define ATA_H
#include <stdint.h>
#include <devices/pci.h>
#include <malloc/malloc.h>
#include <graphics/graphics.h>
#include <sys/portio.hpp>
//For checking and connecting to ATA drives
#define VENDOR_ID 0x8086
#define DEVICE_ID 0x7010

//Drive bus information
#define PRIMARY_BUS 0x00
#define SECONDARY_BUS 0x01

//Master / slave drive information
#define MAIN_DRIVE 0xE0
#define BACKUP_DRIVE 0xF0

//Hard drive select port
#define ATA_HD_SEL 0x06

//All the io base registers
#define ATA_DATA 0x00
#define ATA_ERROR 0x01
#define ATA_FEATURES 0x01
#define ATA_SECTOR_COUNT 0x02
#define ATA_LBA_LOW 0x03
#define ATA_LBA_0   0x03
#define ATA_LBA_1   0x04
#define ATA_LBA_2 0x05
#define ATA_LBA_MID 0x04
#define ATA_LBA_3 0x09
#define ATA_LBA_4 0x0A
#define ATA_LBA_5 0x0B
#define ATA_LBA_HIGH 0x05
#define ATA_COMMAND 0x07
#define ATA_STATUS 0x07
#define ATA_IDENTIFY 0xEC

//Registers offset from control base
#define ATA_ALT_STATUS 0x0C
#define ATA_DEVICE_CONTROL 0x0C
#define ATA_DEVICE_ADDRESS 0x0D

//Status register
#define ATA_BSY 0x80
#define ATA_RDY 0x40
#define ATA_DF 0x20
#define ATA_DSC 0x10
#define ATA_DRQ 0x08
#define ATA_CORR 0x04
#define ATA_IDX 0x02
#define ATA_ERR 0x01
#define ATA_SRST 4

//Error register
#define ADDRESS_MARKER_NOT_FOUND_ERROR 0x00
#define TRACK_ZERO_NOT FOUND_ERROR     0x01
#define ABORTED_COMMAND_ERROR          0x02
#define MEDIA_CHANGE_REQUEST_ERROR     0x03
#define IDENTITY_NOT_FOUND_ERROR       0x04
#define MEDIA_CHANGED_ERROR            0x05
#define UNCORRECTABLE_DATA_ERROR       0x06
#define BAD_BLOCK_DETECTED_ERROR       0x07

//The drive / head register
#define DRIVE_NUMBER_SELECT 0x04
#define DRIVE_LBA           0x06

#define NO_DRIVE 0xFFFF

//Device identifiers
#define ATA_MODEL 54

//Some device commands
#define ATA_COMMAND_READ_PIO 0x20
#define ATA_COMMAND_WRITE_PIO 0x30
#define ATA_FLUSH_CACHE_COMMAND   0xE7

#define ATA_MAX_SECTOR_COUNT  1024

typedef struct _ata_drive
{
    char* drive_name;
    uint8_t bus;
    uint8_t slot;
    uint8_t sector_count;
    uint8_t drive_id;
    uint8_t lba_low;
    uint8_t lba_mid;
    uint8_t lba_high;
    uint16_t lba;
    uint16_t io;
    int descriptor;
    uint8_t (*read)(uint32_t *address,  struct _ata_drive* drive);
    uint8_t (*write)(uint32_t *buffer, struct _ata_drive* drive);
    pci_device *pci_device;
} ata_drive;

void wait_rdy(ata_drive *drive);
void wait_bsy(ata_drive *drive);
uint8_t ata_read_pio(uint32_t *target, ata_drive *drive);
uint8_t ata_write_pio(uint32_t *buffer, ata_drive *drive);
void select_drive(uint8_t bus, uint8_t slot);
uint8_t identify_drive(uint8_t bus, uint8_t drive);
char *get_drive_name(uint8_t bus, uint8_t drive);
ata_drive *make_ata_drive(uint8_t bus, uint8_t slot, uint8_t sector_count);
ata_drive *initialize_ata_driver();
#endif