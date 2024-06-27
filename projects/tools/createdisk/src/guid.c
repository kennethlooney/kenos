#include <system/common/guid.h>


// std
#include <stdlib.h>
// static unsigned int seed = 123456789;

// unsigned int rand() {
//     seed ^= (seed << 13);
//     seed ^= (seed >> 17);
//     seed ^= (seed << 5);
//     return seed;
// }

Guid new_guid()
{
    uint8_t rand_arr[16] = {0};

    for(uint8_t i = 0; i < sizeof rand_arr; i++)
        rand_arr[i] = rand() % (UINT8_MAX + 1);
    
    // Fill out Guid
    Guid result = {
        .time_lo         = *(uint32_t*)&rand_arr[0],
        .time_mid        = *(uint16_t*)&rand_arr[4],
        .time_hi_and_ver = *(uint16_t*)&rand_arr[6],
        .clock_seq_hi_and_res = rand_arr[8],
        .clock_seq_lo = rand_arr[9],
        .node = {rand_arr[10], rand_arr[11], rand_arr[12], rand_arr[13],
            rand_arr[14], rand_arr[15] },
    };
    
    // Fill out version bits
    result.time_hi_and_ver &= ~(1 << 15);
    result.time_hi_and_ver |= (1 << 14);
    result.time_hi_and_ver &= ~(1 << 13); 
    result.time_hi_and_ver &= ~(1 << 12); 
    
    // Fill out variant bits
    result.clock_seq_hi_and_res |= (1 << 7);
    result.clock_seq_hi_and_res |= (1 << 6);
    result.clock_seq_hi_and_res &= ~(1 << 5);
    
    return result;

}