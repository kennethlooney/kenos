#include <system/formats/disk/mbr.h>
#include <system/formats/disk/gpt.h>
#include <system/common/guid.h>
#include <system/common/utils.h>
#include <system/formats/disk/fat.h>

// std
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

const Guid ESP_GUID = {0xC12A7328, 0xF81F, 0x11D2, 0xBA, 0x4B, {0x00, 0xA0, 0xC9, 0x3E, 0xC9, 0x3B}};
const Guid BASIC_DATA_GUID = {0xEBD0A0A2, 0xB9E5, 0x4433, 0x87, 0xC0, {0x68, 0xB6, 0xB7, 0x26, 0x99, 0xC7}};


char* image_name = "disk.img";
uint64_t lba_size = 512;
uint64_t esp_size = 1024*1024*33;       // 33mb
uint64_t data_size = 1024*1024*1;       // 1mb
uint64_t image_size = 0;

uint64_t esp_size_lbas = 0, data_size_lbas = 0, image_size_lbas = 0, gpt_table_lbas = 0; // Sizes in LBAs
uint64_t align_lba = 0, esp_lba = 0, data_lba = 0;  // Starting LBA values








bool write_mbr(FILE* image);
uint64_t bytes_to_lbas(const uint64_t bytes);
void write_full_lba_size(FILE* image);
bool write_gpts(FILE* image);
bool write_esp(FILE* image);


//======================================================================================================
// main
int main(int argc, char** argv)
{
    FILE* image = fopen(image_name, "wb+");
    if(!image) {
        fprintf(stderr, "Error: could not open file %s\n", image_name);
        return EXIT_FAILURE;
    } 

    // Set sizes & LBA values
    gpt_table_lbas = GPT_TABLE_SIZE / lba_size;
    const uint64_t padding = (ALIGNMENT*2 + (lba_size * ((gpt_table_lbas*2) + 1 + 2)));
    image_size = esp_size + data_size + padding;    // Add some extra padding for GPTs/MBR
    image_size_lbas = bytes_to_lbas(image_size);
    align_lba = ALIGNMENT / lba_size;
    esp_lba = align_lba;
    esp_size_lbas = bytes_to_lbas(esp_size);
    data_size_lbas = bytes_to_lbas(data_size);
    data_lba = next_aligned_lba(align_lba, esp_lba + esp_size_lbas);
   
    // Seed random number generator
    srand(time(NULL));

    // Write Protective MBR
    if(!write_mbr(image)) {
        fprintf(stderr, "Error: could not write protective MBR to file %s\n", image_name);
        return EXIT_FAILURE;
    }

    // Write GPT headers & tables
     if(!write_gpts(image)) {
        fprintf(stderr, "Error: could not write GPT headers and tables to file %s\n", image_name);
        return EXIT_FAILURE;
    }

    // Write EFI System Partition w/FAT32 filesystem
    if(!write_esp(image)) {
        fprintf(stderr, "Error: could not write FAT32 filesystem to ESP for file %s\n", image_name);
        return EXIT_FAILURE;
    }

    
    
    printf("Created disk image '%s'\n", image_name);

    return EXIT_SUCCESS;
}
 
//=======================================================================================================
// write_esp
bool write_esp(FILE* image)
{
    // Left off: https://youtu.be/dxDbb87h-Ro?list=PLT7NbkyNWaqZYHNLtOZ1MNxOt8myP5K0p&t=1272
    return true;
}
//=======================================================================================================
// write_mbr
bool write_mbr(FILE* image)
{
    uint64_t mbr_image_lbas = image_size_lbas;
    if(mbr_image_lbas > 0xFFFFFFFF) mbr_image_lbas = 0x100000000;

    Mbr mbr = {
        .boot_code = {0},
        .mbr_signature = 0,
        .unknown = 0,
        .partition[0] = {
            .boot_indicator = 0,
            .starting_chs = {0x00,0x02,0x00},
            .os_type = 0xEE,    // Protective GPT
            .ending_chs = {0xFF,0xFF,0xFF},
            .starting_lba = 0x00000001,
            .size_lba = mbr_image_lbas - 1,
            
        },
        .boot_signature = 0xAA55,

    };
    // Write to disk image 
    if(fwrite(&mbr, 1, sizeof mbr, image) != sizeof mbr)
        return false;
     
    write_full_lba_size(image);

    return true;
}
 

//=======================================================================================================
// bytes_to_lbas
uint64_t bytes_to_lbas(const uint64_t bytes)
{
    return (bytes / lba_size) + (bytes % lba_size > 0 ? 1 : 0);
}
 

//=======================================================================================================
// write_full_lba_size
void write_full_lba_size(FILE* image)
{
    uint8_t zero_sector[512];
    for(uint8_t i = 0; i < (lba_size - sizeof zero_sector) / sizeof zero_sector; i++)
        fwrite(zero_sector, sizeof zero_sector, 1, image);

}



//=======================================================================================================
// write_gpts
bool write_gpts(FILE* image)
{
     // Fill out primary GPT header
    Gpt_Header primary_gpt = {
        .signature = { "EFI PART" },
        .revision = 0x00010000,   // Version 1.0
        .header_size = 92,
        .header_crc32 = 0,      // Will calculate later
        .reserved_1 = 0,
        .my_lba = 1,            // LBA 1 is right after MBR
        .alternate_lba = image_size_lbas - 1,
        .first_usable_lba = 1 + 1 + gpt_table_lbas, // MBR + GPT header + primary gpt table
        .last_usable_lba = image_size_lbas - 1 - gpt_table_lbas - 1, // 2nd GPT header + table
        .disk_guid = new_guid(),
        .partition_table_lba = 2,   // After MBR + GPT header
        .number_of_entries = 128,   
        .size_of_entry = 128,
        .partition_table_crc32 = 0, // Will calculate later
        .reserved_2 = { 0 },
    };

    // Fill out primary table partition entries
    Gpt_Partition_Entry gpt_table[GPT_TABLE_ENTRIES_COUNT] = {
        // EFI System Paritition
        {
            .partition_type_guid = ESP_GUID,
            .unique_guid = new_guid(),
            .starting_lba = esp_lba,
            .ending_lba = esp_lba + esp_size_lbas,
            .attributes = 0,
            .name = u"EFI SYSTEM",
        },

        // Basic Data Paritition
        {
            .partition_type_guid  = BASIC_DATA_GUID,
            .unique_guid = new_guid(),
            .starting_lba = data_lba,
            .ending_lba = data_lba + data_size_lbas,
            .attributes = 0,
            .name = u"BASIC DATA",
        },
    };

    // Fill out primary header CRC values
    primary_gpt.partition_table_crc32 = calculate_crc32(gpt_table, sizeof gpt_table);
    primary_gpt.header_crc32 = calculate_crc32(&primary_gpt, primary_gpt.header_size);

    // Write primary gpt header to file
    if (fwrite(&primary_gpt, 1, sizeof primary_gpt, image) != sizeof primary_gpt)
        return false;
    write_full_lba_size(image);

    // Write primary gpt table to file
    if (fwrite(&gpt_table, 1, sizeof gpt_table, image) != sizeof gpt_table)
        return false;

    // Fill out secondary GPT header
    Gpt_Header secondary_gpt = primary_gpt;

    secondary_gpt.header_crc32 = 0;
    secondary_gpt.partition_table_crc32 = 0;
    secondary_gpt.my_lba = primary_gpt.alternate_lba;
    secondary_gpt.alternate_lba = primary_gpt.my_lba;
    secondary_gpt.partition_table_lba = image_size_lbas - 1 - gpt_table_lbas;

    // Fill out secondary header CRC values
    secondary_gpt.partition_table_crc32 = calculate_crc32(gpt_table, sizeof gpt_table);
    secondary_gpt.header_crc32 = calculate_crc32(&secondary_gpt, secondary_gpt.header_size);

    // Go to position of secondary table
    fseek(image, secondary_gpt.partition_table_lba * lba_size, SEEK_SET);
    
    // Write secondary gpt table to file
    if (fwrite(&gpt_table, 1, sizeof gpt_table, image) != sizeof gpt_table)
        return false;

    // Write secondary gpt header to file
    if (fwrite(&secondary_gpt, 1, sizeof secondary_gpt, image) != sizeof secondary_gpt)
        return false;
    write_full_lba_size(image);

    return true;
}
 