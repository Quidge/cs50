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

    while (feof(inptr) == 0)
    {
        fread(temp, sizeof(BYTE), 512, inptr);

        char newFileName[50];
        sprintf(newFileName, "%d.JPG", curFile);
        curFile++;

        printf("OI!: %d\n", feof(inptr));

        FILE *newFile = fopen(newFileName, "w");
        if (newFile == NULL)
        {
            fprintf(stderr, "fopen opened to a NULL pointer");
            return 2;
        }
        fwrite(temp, sizeof(BYTE), 512, newFile);
        fclose(newFile);
        printf("%d\n", feof(inptr));
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