#pragma once
#include <stdint.h>

typedef struct {
    uint32_t time_lo;
    uint16_t time_mid;
    uint16_t time_hi_and_ver;           // Highest 4 bits are version #
    uint8_t clock_seq_hi_and_res;       // Highest bits are variant #
    uint8_t clock_seq_lo;
    uint8_t node[6];
}__attribute__((packed)) Guid;

// unsigned int rand();

extern Guid new_guid();