#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Argument should be the filename\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");
    FILE *img = NULL;

    int ctr = 0;
    BYTE buffer[512];

    while (fread(buffer, 1, 512, card) == 512)
    {
        // fread(buffer, 1, 512, card);
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            ((buffer[3] & 0xf0) == 0xe0))
        {
            if (ctr != 0)
            {
                fclose(img);
            }
            char filename[8];
            sprintf(filename, "%03i.jpg", ctr);
            img = fopen(filename, "w");
            ctr++;
            fwrite(buffer, 1, 512, img);
        }
        else if (ctr != 0)
        {
            fwrite(buffer, 1, 512, img);
        }
    }

    fclose(card);
    if (img != NULL)
    {
        fclose(img);
    }
    return 0;
}
