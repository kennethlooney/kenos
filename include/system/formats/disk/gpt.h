#pragma once
#include <system/common/guid.h>
#if __has_include(<uchar.h>)
#include <uchar.h>
#endif

#ifndef _UCHAR_H
typedef uint_least16_t char16_t;
#endif

// std
#include <stdint.h>


enum {
    GPT_TABLE_ENTRY_SIZE = 128,
    GPT_TABLE_ENTRIES_COUNT = 128,
    GPT_TABLE_SIZE = GPT_TABLE_ENTRY_SIZE * GPT_TABLE_ENTRIES_COUNT,    // Minimum size per UEFI spec 2.10
    ALIGNMENT = (1024*1024) * 1,                                        // 1Mb alignment
};

typedef struct {
    uint8_t signature[8];
    uint32_t revision;
    uint32_t header_size;
    uint32_t header_crc32;
    uint32_t reserved_1;
    uint64_t my_lba;
    uint64_t alternate_lba;
    uint64_t first_usable_lba;
    uint64_t last_usable_lba;
    Guid disk_guid;
    uint64_t partition_table_lba;
    uint32_t number_of_entries;
    uint32_t size_of_entry;
    uint32_t partition_table_crc32;

    uint8_t reserved_2[512-92];
}__attribute__((packed)) Gpt_Header;

typedef struct {
    Guid partition_type_guid;
    Guid unique_guid;
    uint64_t starting_lba;
    uint64_t ending_lba;
    uint64_t attributes;
    char16_t name[36];
}__attribute__((packed)) Gpt_Partition_Entry;