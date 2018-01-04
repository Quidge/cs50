#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* General idea:
 * - Move through the file byte by byte till fingerprint region is found.
 * - Rewind so that the very next byte is the start of the fingerprint region.
 *      - Copy 512 bytes from the file, at current file position. Repeat
 *      until 512 byte block is found that AGAIN begins with a JPG byte
 *      fingerprint region. This boundary is the end of one JPG and the
 *      beginning of another.
 * - Repeat again, copying information into a new file.
 */


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

    //temp fingerprint region holder
    BYTE fingp[] = {0,0,0,0};

    // read bytes into fingp.
    // compare each series of bytes in fingp with JPEG fingerprint
    // if fingerprints match, assign found to 1
    int found = 0;
    while (found == 0 && feof(inptr) == 0)
    {
        fingp[0] = fingp[1];
        fingp[1] = fingp[2];
        fingp[2] = fingp[3];
        fread(&fingp[3], sizeof(BYTE), 1, inptr);

        if (fingp[0] == 0xff && fingp[1] == 0xd8 &&
            fingp[2] == 0xff &&
            (fingp[3] >= 0xe0 && fingp[3] <= 0xef))
        {
            found = 1;
            fseek(inptr, -4, SEEK_CUR);
        }
    };

    // If no JPG byte signature is found, exit the file.
    if (found == 0)
    {
        fprintf(stdout, "No evidence of JPG files found in %s\n", argv[1]);
        return 3;
    }

    // this will be the incrementor for JPG naming
    int curFile = 0;

    // 512 chunks will be continuously written (and rewritten) to temp
    int *temp = malloc(512);
    if (temp == NULL)
    {
        fprintf(stderr, "malloc could not find space.");
        fclose(inptr);
        return 2;
    }

    while (feof(inptr) == 0)
    {
        // Get new file ready with name, run check to be sure
        // it isn't a NULL pointer
        char newFileName[50];
        sprintf(newFileName, "%03d.jpg", curFile);
        curFile++;

        FILE *newFile = fopen(newFileName, "w");
        if (newFile == NULL)
        {
            fprintf(stderr, "fopen opened to a NULL pointer");
            fclose(newFile);
            fclose(inptr);
            free(temp);
            return 2;
        }

        // write the first 512 byte block into the new JPG
        fread(temp, sizeof(BYTE), 512, inptr);
        fwrite(temp, sizeof(BYTE), 512, newFile);

        // next section determines if the file continues on the recovery
        // section, and keeps going if so

        do
        {
            fread(temp, sizeof(BYTE), 512, inptr);
            fwrite(temp, sizeof(BYTE), 512, newFile);

            // zero out fingerprint region
            fingp[0] = 0;
            fingp[1] = 0;
            fingp[2] = 0;
            fingp[3] = 0;
            for (int i = 0; i < 4; i++)
            {
                fingp[0] = fingp[1];
                fingp[1] = fingp[2];
                fingp[2] = fingp[3];
                fread(&fingp[3], sizeof(BYTE), 1, inptr);
            }
            if (feof(inptr) == 0)
            {
                fseek(inptr, -4, SEEK_CUR);
            }
        }
        while (!(fingp[0] == 0xff && fingp[1] == 0xd8 &&
                fingp[2] == 0xff &&
                (fingp[3] >= 0xe0 && fingp[3] <= 0xef)) &&
                feof(inptr) == 0);

        fclose(newFile);
    }

    free(temp);
    fclose(inptr);
}