#include <bmp.h>

bmp_header m_header;

uint8_t *m_data = NULL;

uint8_t *m_palette = NULL;

enum result_code m_status = no_action;

void bmp_read(const char *input)
{
    m_status = no_action;

    FILE *file = fopen(input, "rb");
    if (file == NULL)
    {
        m_status = cannot_open_bmp;

        return;
    }

    fread(&m_header, sizeof(m_header), 1, file);

    m_palette = malloc(m_header.colors_used * m_header.bit_count / 2);
    fread(m_palette, sizeof(uint8_t), m_header.colors_used * m_header.bit_count / 2, file);

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
    fwrite(m_palette, sizeof(uint8_t), m_header.colors_used * m_header.bit_count / 2, file);
    fwrite(m_data, m_header.size_image, 1, file);

    fclose(file);
    
    m_status = success;
}

void bmp_8bit_indexed_compress()
{
    const int compressed_factor = 2;

    int compressed_size = 0;
    uint8_t *compressed_data = malloc(m_header.size_image);

    for (int y = 0; y < m_header.height; y++)
    {
        for (int x = 0; x < m_header.width; x++)
        {
            const uint8_t compress_pixel = m_data[y * m_header.width + x] / compressed_factor;

            compressed_data[compressed_size++] = compress_pixel;
        }
    }

    free(m_data);

    m_data = compressed_data;
    m_header.size_image = compressed_size;

    m_status = success;
}

void bmp_compress()
{
    m_status = no_action;

    if (m_header.compression != 0)
    {
        m_status = already_compressed;

        return;
    }
    
    if (m_header.bit_count == 8)
    {
        bmp_8bit_indexed_compress();
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