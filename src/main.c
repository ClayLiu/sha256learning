#include <stdio.h>
#include <stdlib.h>
#include "sha256.h"

const size_t MB = 1024 * 1024;

void hex_print_byte(unsigned char x);


void print_sha256_digesthex(FILE* fp)
{
    int i;
    size_t read = 0;
    char* input_buffer = malloc(sizeof(char) * 2 * MB);
    char* digest_array = malloc(sizeof(char) * 32);
    sha256_context* context = new_context();

    while(read = fread(input_buffer, sizeof(char), 2 * MB, fp))
    {
        printf("read = %llu\n", read);
        update(context, input_buffer, read);
    }

    digest(context);
    get_digest(context, digest_array);

    for(i = 0; i < 32; i++)
    {
        if(i % 4 == 0)
            putchar(',');
        hex_print_byte(digest_array[i]);
    }

    destroy_context(context);
    free(digest_array);
    free(input_buffer);
}


int main(int argc, char** argv)
{
//    if(argc == 1)
//    {
//        printf("No file is inputing.");
//        return 0;
//    }

    char* filename = "test_files\\temp_sha.txt";

    // FILE* fp = fopen(argv[1], "rb");
    FILE* fp = fopen(filename, "rb");
    print_sha256_digesthex(fp);
//    char t = 0;
//    hex_print_byte(t);
//    putchar('\n');
//
//    fread(&t, sizeof(char), 1, fp);
//    putchar(t);
//    putchar('\n');
//    printf("dec = %d\n", t);
//    hex_print_byte(t);

    fclose(fp);
    return 0;
}
