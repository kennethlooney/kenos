#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint8_t boot_indicator;
    uint8_t starting_chs[3];
    uint8_t os_type;
    uint8_t ending_chs[3];
    uint32_t starting_lba;
    uint32_t size_lba;
}__attribute__((packed)) Mbr_Partition;

typedef struct {
    uint8_t boot_code[440];
    uint32_t mbr_signature;
    uint16_t unknown;
    Mbr_Partition partition[4];
    uint16_t boot_signature;    
}__attribute__((packed)) Mbr;


bool write_mbr(FILE* image, uint64_t image_size_lbas);
uint64_t bytes_to_lbas(const uint64_t bytes, uint64_t lba_size);