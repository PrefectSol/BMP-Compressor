#include <bmp.h>

int encode(const char *input, const char *output)
{
    bmp_read(input);
    if (m_status != success)
    {
        printf("bmp_read error: %d", m_status);
        
        return m_status;
    }

    bmp_compress();
    if (m_status != success)
    {
        printf("bmp_compress error: %d", m_status);
        
        return m_status;
    }

    bmp_write(output);
    if (m_status != success)
    {
        printf("bmp_write error: %d", m_status);
        
        return m_status;
    }

    bmp_free();
    if (m_status != success)
    {
        printf("bmp_free error: %d", m_status);
        
        return m_status;
    }

    printf("success: %d", m_status);

    return 0;
}

int main(int argc, const char **argv) 
{
    if (argc == 3)
    {
        return encode(argv[1], argv[2]);
    }

    printf(".bmp compressor \nExample: \nmain.c <input.bmp> <output.bmp>");
        
    return 1;
}
