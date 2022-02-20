#include "logic_functions.h"

/* 循环右移 */
#ifdef __GNUC__
uint32_t s(uint32_t x, uint8_t n)
{
    uint32_t result;
    __asm__
    (
        "movb %2, %%cl\n\t" \
        "ror %%cl, %1\n\t" \
        "movl %1, %0"
        :"=r"(result)
        :"r"(x), "r"(n)
        :"%cl"
    );
    return result;
}
#else
const uint32_t mask_array[32] = {
    0x00000000u, 0x00000001u, 0x00000003u, 0x00000007u, 
    0x0000000fu, 0x0000001fu, 0x0000003fu, 0x0000007fu,
    0x000000ffu, 0x000001ffu, 0x000003ffu, 0x000007ffu,
    0x00000fffu, 0x00001fffu, 0x00003fffu, 0x00007fffu,
    0x0000ffffu, 0x0001ffffu, 0x0003ffffu, 0x0007ffffu,
    0x000fffffu, 0x001fffffu, 0x003fffffu, 0x007fffffu,
    0x00ffffffu, 0x01ffffffu, 0x03ffffffu, 0x07ffffffu,
    0x0fffffffu, 0x1fffffffu, 0x3fffffffu, 0x7fffffffu
};

uint32_t s(uint32_t x, uint8_t n)
{
    n %= 32;
    uint32_t mask = mask_array[n];

    return ((x & mask) << (32 - n)) | ((x & ~mask) >> n);
}
#endif


uint32_t ch(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ ((~x) & z);
}


uint32_t maj(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ (x & z) ^ (y & z);
}


uint32_t big_sigma_0(uint32_t x)
{
    return s(x, 2) ^ s(x, 13) ^ s(x, 22);
}


uint32_t big_sigma_1(uint32_t x)
{
    return s(x, 6) ^ s(x, 11) ^ s(x, 25);
}


uint32_t sigma_0(uint32_t x)
{
    return s(x, 7) ^ s(x, 18) ^ (x >> 3);
}


uint32_t sigma_1(uint32_t x)
{
    return s(x, 17) ^ s(x, 19) ^ (x >> 10);
}