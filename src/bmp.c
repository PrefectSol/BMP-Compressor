#include <bmp.h>

bmp_header m_header;

uint8_t *m_data = NULL;
uint8_t *m_palette = NULL;

enum result_code m_status = no_action;

void bmp_read(const char *input)
{
    const unsigned int bmp_filetype = 0x4D42;

    m_status = no_action;

    FILE *file = fopen(input, "rb");
    if (file == NULL)
    {
        m_status = cannot_open_bmp;

        return;
    }

    fread(&m_header, sizeof(m_header), 1, file);
    if (m_header.file_type != bmp_filetype)
    {
        m_status = incorrect_file_type;

        return;
    }

    m_palette = malloc(m_header.colors_used);
    if (m_palette == NULL)
    {
        m_status = memory_allocation_error;

        return;
    }

    fread(m_palette, sizeof(uint8_t), m_header.colors_used, file);

    m_data = malloc(m_header.size_image);
    if (m_data == NULL)
    {
        m_status = memory_allocation_error;

        return;
    }

    fread(m_data, m_header.size_image, 1, file);

    fclose(file);

    m_status = success;
}

void bmp_write(const char *output)
{
    m_status = no_action;

    FILE *file = fopen(output, "wb");
    if (file == NULL)
    {
        m_status = cannot_open_bmp;

        return;
    }

    fwrite(&m_header, sizeof(m_header), 1, file);
    fwrite(m_palette, sizeof(uint8_t), m_header.colors_used, file);
    fwrite(m_data, m_header.size_image, 1, file);

    fclose(file);
    
    m_status = success;
}

void bmp_indexed_compress(unsigned int byteSize, uint32_t compression)
{
    uint8_t *compressed_data = malloc(m_header.size_image);
    uint8_t *comp_data_ptr = compressed_data;

    for (int row = 0; row < m_header.height; row++)
    {
        uint8_t *row_ptr = m_data + row * m_header.width;
        const uint8_t *row_end = row_ptr + m_header.width;

        while (row_ptr < row_end)
        {
            unsigned int count = 1;
            const uint8_t pixel = *row_ptr++;

            while (row_ptr < row_end && *row_ptr == pixel && count < byteSize)
            {
                count++;
                row_ptr++;
            }

            *comp_data_ptr++ = count;
            *comp_data_ptr++ = pixel;
        }
    }

    m_header.compression = compression;
    m_header.size_image = comp_data_ptr - compressed_data;

    free(m_data);
    m_data = compressed_data;

    m_status = success;
}

void bmp_compress()
{
    m_status = no_action;

    if (m_header.compression != BI_RGB)
    {
        m_status = already_compressed;

        return;
    }
    
    if (m_header.bit_count == 8)
    {
        bmp_indexed_compress(255, BI_RLE8);
    }
    else if (m_header.bit_count == 4)
    {
        bmp_indexed_compress(128, BI_RLE4);
    }
    else
    {
        m_status = not_indexed_color_scheme;
    }
}

void bmp_free()
{
    m_status = no_action;

    if (m_data != NULL)
    {
        free(m_data);

        m_status = success;

        return;
    }

    if (m_palette != NULL)
    {
        free(m_palette);

        m_status = success;

        return;
    }

    m_status = free_not_required;
}