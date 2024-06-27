#pragma once

#include <stdint.h>
#include <stdbool.h>

void create_crc32_table();
uint32_t calculate_crc32(void* buf, int32_t len);
uint64_t next_aligned_lba(const uint64_t align_lba, const uint64_t lba);