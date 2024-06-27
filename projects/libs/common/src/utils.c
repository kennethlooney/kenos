#include <system/common/utils.h>

uint32_t crc32_table[256];
//=======================================================================================================
// create_crc32_table
void create_crc32_table() {
    uint32_t c = 0;
    
    for(int32_t n = 0; n < 256; n++) {
        c = (uint32_t)n;
        for(int32_t k = 0; k < 8; k++) {
            if(c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        crc32_table[n] = c;
    }
}

//=======================================================================================================
// create_crc32_table
uint32_t calculate_crc32(void* buf, int32_t len) {
    static bool made_crc_table = false;

    uint8_t *bufp = buf;
    uint32_t c = 0xFFFFFFFFL;

    if(!made_crc_table) {
        create_crc32_table();
        made_crc_table = true;
    }

    for(int32_t n = 0; n < len; n++)
        c = crc32_table[(c ^ bufp[n]) & 0xFF] ^ (c >> 8);

    return c ^ 0xFFFFFFFFL;
}

//=============================================================================================================
// next_aligned_lba
uint64_t next_aligned_lba(const uint64_t align_lba, const uint64_t lba)
{
    return lba - (lba % align_lba) + align_lba;
}
