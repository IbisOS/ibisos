#include <storage/ata.h>
//ATA buffer
uint8_t *ata_buffer;
//Get PCI device
pci_device *pci_dev = get_discovered_device(VENDOR_ID, DEVICE_ID);

void wait_rdy(ata_drive *drive)
{
    //Drive IO
    uint16_t drive_io = (drive->bus == PRIMARY_BUS) ? pci_dev->bar0 : pci_dev->bar2;
    //Wait for bsy bit to clear
    while(inb(drive_io + ATA_STATUS) & ATA_RDY);
}

void wait_bsy(ata_drive *drive)
{
    //Drive IO
    uint16_t drive_io = (drive->bus == PRIMARY_BUS) ? pci_dev->bar0 : pci_dev->bar2;
    //Wait for bsy bit to clear
    while(inb(drive_io + ATA_STATUS) & ATA_BSY);
}

uint8_t ata_read_pio(uint32_t *target, ata_drive *drive)
{
    //Wait for bsy bit to clear
    wait_bsy(drive);
    //Select the drive
    outb(drive->io + ATA_HD_SEL, drive->slot | ((drive->lba >> 24) & 0xF));
    //Write the sector count
    outb(drive->io + ATA_SECTOR_COUNT, drive->sector_count);
    //Write the low LBA
    outb(drive->io + ATA_LBA_LOW, (uint8_t) drive->lba_low);
    //Write the mid LBA
    outb(drive->io + ATA_LBA_MID, (uint8_t) drive->lba_mid);
    //Write the high LBa
    outb(drive->io + ATA_LBA_HIGH, (uint8_t) drive->lba_high);
    //Tell the drive that we are ready to read
    outb(drive->io + ATA_COMMAND, ATA_COMMAND_READ_PIO);
    //Loop through the sectors
    for(int sector = drive->descriptor; sector < drive->sector_count; sector++)
    {
        //Wait for bsy bit to clear
        wait_bsy(drive);
        //Wait for rdy to clear
        wait_rdy(drive);
        //Loop 256 times
        for(int i = 0; i < 256; i++)
        {
            //Set target at i
            target[i] = inw(drive->io + ATA_DATA);
        }
        //Add 256 to the target
        target += 256;
    }
}

uint8_t ata_write_pio(uint32_t *buffer, ata_drive *drive)
{
    //Wait for bsy
    wait_bsy(drive);
    //Select the drive for 24-bit writing
    outb(drive->io + ATA_HD_SEL, drive->slot | ((drive->lba >> 24) & 0xF));
    //Write the current sector count
    outb(drive->io + ATA_SECTOR_COUNT, drive->sector_count);
    //Write the low LBA
    outb(drive->io +ATA_LBA_LOW, (uint8_t) drive->lba_low);
    //Write the mid LBA
    outb(drive->io + ATA_LBA_MID, (uint8_t) drive->lba_mid);
    //Write the high LBA
    outb(drive->io + ATA_LBA_HIGH, (uint8_t) drive->lba_high);
    //Notify the drive that we want to write
    outb(drive->io + ATA_COMMAND, ATA_COMMAND_WRITE_PIO);
    //Loop through sector count
    for (int sector = drive->descriptor; sector < drive->sector_count; sector++)
    {
        //Wait for BSY bit to clear
        wait_bsy(drive);
        //Wait for RDY bit to clear
        wait_rdy(drive);
        //Loop for 256 times
        for(int idx = 0; idx < 256; idx++)
        {
            //Set target's index to data from drive
            outl(drive->io + ATA_DATA, buffer[idx]);
        }
    }
}

void select_drive(uint8_t bus, uint8_t slot)
{
    if(bus == PRIMARY_BUS)
    {
        if(slot == MAIN_DRIVE)
            //Select the given main drive
            outb(pci_dev->bar0 + ATA_HD_SEL, MAIN_DRIVE);
        else
            //Select a backup drive
            outb(pci_dev->bar2 + ATA_HD_SEL, BACKUP_DRIVE);
    }
    else
    {
        if(slot == MAIN_DRIVE)
            //Select the main drive
            outb(pci_dev->bar0 + ATA_HD_SEL, MAIN_DRIVE);
        else
            //Select the backuo drive
            outb(pci_dev->bar2 + ATA_HD_SEL, BACKUP_DRIVE);
    }
}

uint8_t identify_drive(uint8_t bus, uint8_t drive)
{
    //Select the given drive
    select_drive(bus, drive);
    //Set the IO
    uint16_t io = (bus == PRIMARY_BUS) ? pci_dev->bar0 : pci_dev->bar1;
    //Set sector count to 0
    outb(io + ATA_SECTOR_COUNT, 0);
    //Set LBA_LOW to 0
    outb(io + ATA_LBA_LOW, 0);
    //Set LBA_MID to 0
    outb(io + ATA_LBA_MID, 0);
    //Set LBA_HIGH to 0
    outb(io + ATA_LBA_HIGH, 0);
    //Send identify command to command port
    outb(io + ATA_COMMAND, ATA_IDENTIFY);
    //Read the status from the drive
    uint8_t status = inb(io + ATA_STATUS);
    //Check if status is valid
    if(status)
    {
        //Check if an error occurred
        if(status & ATA_ERR)
        {
            //No drive
            return NO_DRIVE;
        }
        //Wait until bsy bit is clear
        while((inb(io + ATA_STATUS)) & ATA_BSY);
        //Status read loop for bsy clearing
status_loop: status = inb(io + ATA_STATUS);
        //While DRQ is set, re-read status
        while(!(status & ATA_DRQ)) goto status_loop;
        //Read the drive's data
        for(int sector = 0; sector < 256; sector++)
        {
            //Read into ata buffer
            *(uint16_t *)(ata_buffer + (sector * 2)) = inw(io + ATA_DATA);
        }
    }
    //Drive exists, mi amour
    return io;
}

char *get_drive_name(uint8_t bus, uint8_t drive)
{
    //Check if the drive exists
    if(identify_drive(bus, drive) != NO_DRIVE)
    {
        //One string to rule them all
        char *drive_data = (char*) malloc(sizeof(char*));
        //And in the darkness buffer them
        char *drive_buffer = (char*) malloc(sizeof(char*));
        //Loop through buffer
        for(int idx = 0; idx < sizeof(drive_data); idx += 2)
        {
            //Read future data
            drive_buffer[idx] = ata_buffer[ATA_MODEL + idx + 1];
            //Set the future to the present
            drive_buffer[idx + 1] = ata_buffer[ATA_MODEL + idx];
        }
        //Reset ata buffer
        ata_buffer = 0;
        //Return the drive's buffer
        return drive_buffer;
    }
    //No drive is available
    return (char*) NO_DRIVE;
}

ata_drive *make_ata_drive(uint8_t bus, uint8_t slot, uint8_t sector_count)
{
    //Create an ATA drive instance
    ata_drive *drive = (ata_drive*) malloc(sizeof(ata_drive));
    //Set the IO
    uint16_t io = pci_dev->bar0;
    //Create drive id
    uint8_t drive_id = (bus << 1) | slot;
    //Create the base address
    uint16_t base_address = drive_id ? PRIMARY_BUS : SECONDARY_BUS;
    //Create the base linear block address
    uint16_t lba = base_address;
    //Set drive's name
    drive->drive_name = get_drive_name(bus, slot);
    //Set drive's bus
    drive->bus = bus;
    //Set drive's slot
    drive->slot = slot;
    //Set drive's sector count
    drive->sector_count = sector_count;
    //Set the drive's descriptor
    drive->descriptor = drive->sector_count;
    //Set drive's id
    drive->drive_id = drive_id;
    //Set the drive's base linear block address
    drive->lba = lba;
    //Set drive's low LBA
    drive->lba_low = lba;
    //Set drive's mid LBA
    drive->lba_mid = (lba >> 8);
    //Set drive's high LBA
    drive->lba_high = (lba >> 16);
    //Set the io
    drive->io = io;
    //Set read function
    drive->read = ata_read_pio;
    //Set write function
    drive->write = ata_write_pio;
    //Set the pci device
    drive->pci_device = pci_dev;
}

ata_drive *initialize_ata_driver()
{
    //Ata drive to be selected
    ata_drive * drive;
    //Check if the main drive exists on primary bus
    if(identify_drive(PRIMARY_BUS, MAIN_DRIVE) != NO_DRIVE)
    {
        //Make a drive
        drive = make_ata_drive(PRIMARY_BUS, MAIN_DRIVE, 1);
    }
    else if(identify_drive(PRIMARY_BUS, BACKUP_DRIVE) != NO_DRIVE)
    {
        //Make a drive
        drive = make_ata_drive(PRIMARY_BUS, BACKUP_DRIVE, 1);
    }
    return drive;
}