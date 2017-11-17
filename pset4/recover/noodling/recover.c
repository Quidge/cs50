#include <stdio.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: <./script> <file>\n");
        return 1;
    }
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not open %s\n", argv[1]);
        return 2;
    }

    /*printf("%i", feof(inptr));
    fclose(inptr);*/

    BYTE fingp[] = {0,0,0,0};

    // read until signature is identified
    int found = 0;
    while (found == 0 && feof(inptr) == 0)
    {
        fingp[3] = fingp[2];
        fingp[2] = fingp[1];
        fingp[1] = fingp[0];
        fread(fingp, sizeof(BYTE), 1, inptr);

        if (fingp[0] == 0xff && fingp[1] == 0xd8 &&
            fingp[2] == 0xff &&
            fingp[3] >= 0xe0 && fingp[3] <= 0xef)
        {
            found = 1;
            printf("Found it.\n");
            fclose(inptr);
        }
        //printf("%i, %i, %i, %i\n", fingp[0], fingp[1], fingp[2], fingp[3]);
    };
    if (found == 0)
    {
        printf("didnt find it\n");
    } else
    {
        printf("we got it\n");
    }
}