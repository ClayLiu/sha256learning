#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sha256.h"
#include "logic_functions.h"

#define bigend_affix_length // 以最大端方式放置长度

const size_t chunk_bits = 512;
const size_t chunk_size = 512 / 8 * sizeof(uint8_t);
const size_t hash_size = 8 * sizeof(uint32_t);

const uint32_t origin_k_array[8] = {
    0x6a09e667,
    0xbb67ae85,
    0x3c6ef372,
    0xa54ff53a,
    0x510e527f,
    0x9b05688c,
    0x1f83d9ab,
    0x5be0cd19
};

const uint32_t k_array[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};



sha256_context* new_context()
{
    sha256_context* context = malloc(sizeof(sha256_context));
    context->buffer = malloc(chunk_size);
    context->buffer_size = 0;
    context->length = 0;

    memcpy(context->hash_array, origin_k_array, sizeof(uint32_t) * 8);

    // build_mask();    // don's need to build anymore
                        // since mask_array is coded in hard-code
    return context;
}


void destroy_context(sha256_context* context)
{
    free(context->buffer);
    free(context);
}

static void print_iteration_array(uint32_t* iteration_array)
{
    int i;
    for(i = 0; i < 8; i++)
        printf(", %c : %#0.8x" + (!i) * 2, 'a' + i, iteration_array[i]);
    putchar('\n');
}

static void update_chunk(sha256_context* context)
{
    /*
     * The size of chunk
     * must be 64 Bytes
     */

    enum iteration_index {
        a, b, c, d, e, f, g, h
    };

    int i;
    uint32_t    ch_value,
                big_sigma_0_value,
                big_sigma_1_value,
                ma_value;

    uint32_t* h_array = context->hash_array;
    uint32_t iteration_array[8], t1, t2;

    uint32_t w_array[64];
    
    memcpy(iteration_array, h_array, hash_size);    // make a, b, c, ..., h = h_array

    memcpy(w_array, context->buffer, chunk_size);   // 8 lines since here is making w_array
    for(i = 16; i < 64; i++)
    {
        w_array[i] =  sigma_1(w_array[i - 2])
                    + w_array[i - 7]
                    + sigma_0(w_array[i - 15])
                    + w_array[i - 16];
    }

    for(i = 0; i < 64; i++)
    {
        t1 =      iteration_array[h] 
                + big_sigma_1(iteration_array[e])
                + ch(iteration_array[e], iteration_array[f], iteration_array[g])
                + k_array[i] + w_array[i];

        t2 =      big_sigma_0(iteration_array[a])
                + maj(iteration_array[a], iteration_array[b], iteration_array[c]);
        
        // print_iteration_array(iteration_array);
        memmove(iteration_array + 1, iteration_array, 7 * sizeof(uint32_t));
        
        iteration_array[e] += t1;
        iteration_array[a] = t1 + t2;
        // print_iteration_array(iteration_array);
    }

    for(i = 0; i < 8; i++)
        h_array[i] += iteration_array[i];

    context->buffer_size = 0;
}



void update(sha256_context* context, uint8_t* data, size_t data_size)
{
    context->length += data_size * 8;
    size_t want_size = chunk_size - context->buffer_size;

    while(data_size > want_size)
    {
        printf("want_size = %u\n", (unsigned int)want_size);
        memcpy
        (
            context->buffer + context->buffer_size,
            data,
            want_size
        );

        update_chunk(context);

        data += want_size;
        data_size -= want_size;
        want_size = chunk_size - context->buffer_size;
    }

    memcpy
    (
        context->buffer + context->buffer_size,
        data,
        data_size
    );
    context->buffer_size += data_size;
}


#ifdef bigend_affix_length
    #define affix_length(chunk, length_address)         \
    {                                                   \
        chunk[chunk_size - 1] = *(length_address + 0);  \
        chunk[chunk_size - 2] = *(length_address + 1);  \
        chunk[chunk_size - 3] = *(length_address + 2);  \
        chunk[chunk_size - 4] = *(length_address + 3);  \
        chunk[chunk_size - 5] = *(length_address + 4);  \
        chunk[chunk_size - 6] = *(length_address + 5);  \
        chunk[chunk_size - 7] = *(length_address + 6);  \
        chunk[chunk_size - 8] = *(length_address + 7);  \
    }
#else
    #define affix_length(chunk, length_address) memcpy(chunk + chunk_size - 8, length_address, 8 * sizeof(uint8_t))
#endif

#define put_hex(x)              \
{                               \
    if(0 <= (x) && (x) <= 9)    \
        putchar((x) + '0');     \
    else                        \
        putchar((x) + 'a' - 10);\
}

void hex_print_byte(unsigned char x)
{
    unsigned char high_part = (x & 0xf0) >> 4;
    unsigned char low_part = x & 0xf;

    put_hex(high_part);
    put_hex(low_part);
}

void digest(sha256_context* context)
{
    printf("length = %llu\n", context->length);
    printf("buffer_size = %llu\n", context->buffer_size);

    uint8_t* chunk = NULL;
    uint8_t* length_address = NULL;

    memset
    (
        context->buffer + context->buffer_size,
        0,
        chunk_size - context->buffer_size
    );

    if(context->buffer_size <= chunk_size - 1 - 8)  // enough space to affix tail and length
    {
        context->buffer[context->buffer_size] = 0x80;
        chunk = context->buffer;
        length_address = (uint8_t*)&(context->length);

        for(int i = 0; i < 8; i++)
            hex_print_byte(*(length_address + i));
        putchar('\n');

        affix_length(chunk, length_address);
        for(int i = 0; i < 8; i++)
            hex_print_byte(*(chunk + chunk_size - 8 + i));
        putchar('\n');
        printf("buffer is\n");
        for(int i = 0; i < 64; i++)
            hex_print_byte(*(chunk + i));

        putchar('\n');
        
        update_chunk(context);
    }
    else if(context->buffer_size <= chunk_size - 1) // enough space to affix tail only
    {
        context->buffer[context->buffer_size] = 0x80;
        update_chunk(context);

        digest(context);
    }
    else                                            // buffer hold 512 bits data already
    {
        update_chunk(context);
        digest(context);
    }
}


void get_digest(sha256_context* context, uint8_t* digest_array)
{
    int i;
    putchar('\n');
    print_iteration_array(context->hash_array);
    putchar('\n');

    for(i = 0; i < 32; i++)
        digest_array[i] = context->hash_array[i >> 2] >> (8 * (3 - (i & 3)));
}


//uint main()
//{
//    build_mask();
//    length = 0x12ff;
//    char* length_address;
//    length_address = &length;
//
//    printf("%x, %x", *(length_address + 0), *(length_address + 1));
//    return 0;
//}
