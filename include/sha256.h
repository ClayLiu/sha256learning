#ifndef SHA256_H_INCLUDED
#define SHA256_H_INCLUDED
#include <stdio.h>
#include <stdint.h>

typedef struct sha256_context
{
    uint8_t* buffer;
    size_t buffer_size;

    uint32_t hash_array[8];
    uint64_t length;  // whole message length in bits
}sha256_context;


sha256_context* new_context();

void destroy_context(sha256_context* context);

void update(sha256_context* context, uint8_t* data, size_t data_size);

void digest(sha256_context* context);

void get_digest(sha256_context* context, uint8_t* digest_array);

#endif // SHA256_H_INCLUDED
