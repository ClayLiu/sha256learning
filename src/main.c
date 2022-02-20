#include <stdio.h>
#include <stdlib.h>
#include "sha256.h"

const size_t MB = 1024 * 1024;

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

void print_sha256_digesthex(FILE* fp)
{
    int i;
    size_t read = 0;
    char* input_buffer = malloc(sizeof(char) * 2 * MB);
    char* digest_array = malloc(sizeof(char) * 32);
    sha256_context* context = new_context();

    while(read = fread(input_buffer, sizeof(char), 2 * MB, fp))
        update(context, input_buffer, read);

    digest(context);
    get_digest(context, digest_array);

    for(i = 0; i < 32; i++)
        hex_print_byte(digest_array[i]);

    destroy_context(context);
    free(digest_array);
    free(input_buffer);
}


int main(int argc, char** argv)
{
    if(argc == 1)
    {
       printf("No file is inputing.");
       return 0;
    }

    FILE* fp = fopen(argv[1], "rb");
    print_sha256_digesthex(fp);
    fclose(fp);
    return 0;
}
