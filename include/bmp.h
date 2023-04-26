#ifndef BMP_H
#define BMP_H

#define BI_RGB  0
#define BI_RLE8 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;

#pragma pack(push, 1)
typedef struct bmp_header 
{
    uint16_t file_type;

    uint32_t file_size;

    uint16_t reserved1;
    uint16_t reserved2;

    uint32_t offset_data;
    uint32_t size_header;

    int32_t width;
    int32_t height;

    uint16_t planes;
    uint16_t bit_count;

    uint32_t compression;
    uint32_t size_image;

    int32_t x_pixels_per_meter;
    int32_t y_pixels_per_meter;
    
    uint32_t colors_used;
    uint32_t colors_important;
} bmp_header;
#pragma pack(pop)

enum result_code
{
    success = 0,

    no_action,

    cannot_open_bmp,

    incorrect_file_type,

    free_not_required,

    memory_allocation_error,

    already_compressed,

    not_indexed_color_scheme,
};

extern enum result_code m_status;

extern uint8_t *m_data;

void bmp_read(const char *input);

void bmp_write(const char *output);

void bmp_compress();

void bmp_free();

#endif // !BMP_H