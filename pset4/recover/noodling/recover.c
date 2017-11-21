#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t BYTE;

//int fingpJPG(int target, int fingerprint[]);

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

    // *read bytes into fingp.
    // *compare each series of bytes in fingp with JPEG fingerprint
    // *if fingerprints match, assign found to 1
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
            printf("Found it.\n");
            printf("%li\n",ftell(inptr));
            fseek(inptr, -4, SEEK_CUR);
            printf("%li\n",ftell(inptr));
        }
        //printf("%x|, %x|, %x|, %x\n", fingp[0], fingp[1], fingp[2], fingp[3]);
    };
    printf("%i, %i, %i, %i\n", fingp[0], fingp[1], fingp[2], fingp[3]);

    if (found == 1)
    {
        printf("we go it!\n");
    } else
    {
        printf("didn't find it\n");
    }

    // now that i have inptr file position set to where the first jpeg
    // begins, i can begin writing 512 byte blocks to new files

    // this will be the incrementor for JPG naming
    int curFile = 0;

    // 512 chunks will be continuously written (and rewritten) to temp
    int *temp = malloc(512);
    if (temp == NULL)
    {
        fprintf(stderr, "malloc could not find space.");
        return 2;
    }

    /*fingp[0], fingp[1], fingp[2], fingp[3] = 0;
    for (int i = 0; i < 4 && feof(inptr) == 0; i++)
    {
        fingp[0] = fingp[1];
        fingp[1] = fingp[2];
        fingp[2] = fingp[3];
        fread(&fingp[3], sizeof(BYTE), 1, inptr);
    }
    fseek(inptr, -4, SEEK_CUR);
    printf("before read: %i, %i, %i, %i\n", fingp[0], fingp[1], fingp[2], fingp[3]);

    fread(temp, sizeof(BYTE), 512, inptr);
    fwrite(temp, sizeof(BYTE), 512, newFile);

    fingp[0], fingp[1], fingp[2], fingp[3] = 0;
    for (int i = 0; i < 4 && feof(inptr) == 0; i++)
    {
        fingp[0] = fingp[1];
        fingp[1] = fingp[2];
        fingp[2] = fingp[3];
        fread(&fingp[3], sizeof(BYTE), 1, inptr);
    }
    fseek(inptr, -4, SEEK_CUR);
    printf("after read: %i, %i, %i, %i\n", fingp[0], fingp[1], fingp[2], fingp[3]);*/

    while (feof(inptr) == 0)
    {
        // Get new file ready with name, run check to be sure
        // it isn't a NULL pointer
        char newFileName[50];
        sprintf(newFileName, "%d.JPG", curFile);
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

            //fingp[0], fingp[1], fingp[2], fingp[3] = 0;
            fingp[0] = 0;
            fingp[1] = 0;
            fingp[2] = 0;
            fingp[3] = 0;
            for (int i = 0; i < 4 && feof(inptr) == 0; i++)
            {
                fingp[0] = fingp[1];
                fingp[1] = fingp[2];
                fingp[2] = fingp[3];
                fread(&fingp[3], sizeof(BYTE), 1, inptr);
            }
            fseek(inptr, -4, SEEK_CUR);
        }
        while (!(fingp[0] == 0xff && fingp[1] == 0xd8 &&
                fingp[2] == 0xff &&
                (fingp[3] >= 0xe0 && fingp[3] <= 0xef)));

        fclose(newFile);
    }

    free(temp);
    fclose(inptr);
    printf("%d", curFile);
}

// search through the target, for byteDist steps
// if a series of 4 bytes in target match series present in fingerprint,
// return position where fingerprint started in target
/*int fingpJPG(int target[], int byteDist, int fingerprint[4])
{
    int pos = 0;
    while (position + 3 < byteDist)
    {
        if (target[pos    ] == fingerprint[0] &&
            target[pos + 1] == fingerprint[1] &&
            target[pos + 2] == fingerprint[2] &&
            target[pos + 3] >= 0xe8 && target[pos + 3] <= 0xef)
        {
           return pos;
        }
    }
    return -1;
}*/