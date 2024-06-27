#include <system/formats/disk/mbr.h>

// std
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


char* image_name = "disk.img";
uint64_t lba_size = 512;
uint64_t esp_size = 1024*1024*33;       // 33mb
uint64_t data_size = 1024*1024*1;       // 1mb
uint64_t image_size = 0;

uint64_t esp_lbas = 0, data_lbas = 0, image_size_lbas = 0;







bool write_mbr(FILE* image);
uint64_t bytes_to_lbas(const uint64_t bytes);


//======================================================================================================
// main
int main(int argc, char** argv)
{
    FILE* image = fopen(image_name, "wb+");
    if(!image) {
        fprintf(stderr, "Error: could not open file %s\n", image_name);
        return EXIT_FAILURE;
    } 

    // Set sizes
    image_size = esp_size + data_size + (1024*1024);    // Add some extra padding for GPTs/MBR
    image_size_lbas = bytes_to_lbas(image_size);

    if(!write_mbr(image)) {
        fprintf(stderr, "Error: could not write protective MBR to file %s\n", image_name);
        return EXIT_FAILURE;
    }



    
    
    printf("Created disk image '%s'\n", image_name);

    return EXIT_SUCCESS;
}
//=======================================================================================================

//=======================================================================================================
// write_mbr
bool write_mbr(FILE* image)
{

    if(image_size_lbas > 0xFFFFFFFF) image_size_lbas = 0x100000000;

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
            .size_lba = image_size_lbas - 1,
            
        },
        .boot_signature = 0xAA55,

    };
    // Write to disk image 
    if(fwrite(&mbr, 1, sizeof mbr, image) != sizeof mbr)
        return false;
     
    return true;
}
//=================================================================================================

//=================================================================================================
// bytes_to_lbas
uint64_t bytes_to_lbas(const uint64_t bytes)
{
    return (bytes / lba_size) + (bytes % lba_size > 0 ? 1 : 0);
}
//=================================================================================================