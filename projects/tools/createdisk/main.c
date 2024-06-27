#include <stdio.h>
#include <stdlib.h>
#include "diskformats/mbr.h"

char* image_name = "disk.img";
uint64_t lba_size = 512;
uint64_t esp_size = 1024*1024*33;   // 33mb
uint64_t data_size = 1024*1024*1;   // 1mb
uint64_t image_size = 0;
uint64_t esp_lbas, data_lbas, image_size_lbas;
int main(int argc, char** argv)
{
    FILE* image = fopen(image_name, "wb+");
    if(!image) {
        fprintf(stderr, "Error: could not open file %s\n", image_name);
        return EXIT_FAILURE;
    } 

    // Set sizes
    image_size = esp_size + data_size + (1024*1024);    // Add some padding
    image_size_lbas = bytes_to_lbas(image_size, lba_size);

    if(!write_mbr(image, image_size_lbas))
    {
        fprintf(stderr, "Error: could not write protective MBR to image '%s'\n", image_name);
        return EXIT_FAILURE;
    }
    


    return EXIT_SUCCESS;
}