#include "diskformats/mbr.h"

uint64_t bytes_to_lbas(const uint64_t bytes, uint64_t lba_size)
{
    return (bytes / lba_size) + (bytes % lba_size > 0 ? 1 : 0);
}
 bool write_mbr(FILE* image, uint64_t image_size_lbas)
 {
    if(image_size_lbas > 0xFFFFFFFF) image_size_lbas = 0x100000000;

    //TODO:
    Mbr mbr = {
        .boot_code = {0},
        .mbr_signature = 0,
        .unknown = 0,
        .partition[0] = {
            .boot_indicator = 0,
            .starting_chs = { 0x00, 0x02, 0x00},
            .os_type = 0xEE, // Protective MBR
            .ending_chs = {0xFF, 0xFF, 0xFF},
            .starting_lba = 0x00000001,
            .size_lba = image_size_lbas - 1,
        },
        .boot_signature = 0xAA55,

    };

    //TODO:
    if(fwrite(&mbr, 1, sizeof(mbr), image) != sizeof(mbr))
        return false;
    

    return true;
 }