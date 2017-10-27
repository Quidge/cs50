#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

/*  Here's what I'm thinking:
*   No decimal enlargements are easy. An f of 4 would mean that the
*   byte dictating color for one pixel will be copied on to (n^2) -1
*   more pixels (expanding out in a grid pattern).
*   What is tricky will be shrinking.
*   The magic number will be the number of infile pixel bytes that should
*   represent a single byte pixel on the outfile. However the value for the
*   new outfile pixel is composited, it must be a composite of that
*   magic number of pixel bytes from the infile.
*   No, screw that.
*   It's about finding the GCF between the two pre/post scale sizes.
*   IE, for a 40x40 image with a .75 reduction, the final image will be
*   30x30. GCF is 10 for 40 and 30. 40/10 = 4, 30/10 = 3. So 4 pixels
*   from the infile must dictate 3 pixels of the outfile.
*/

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <floating point between 0 and 100 exclusive> <infile> <outfile>\n", argv[1]);
        return 1;
    }

    // Set up file objects
    char *infile = argv[2];
    char *outfile = argv[3];

    // Run validation on files
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open '%s'.\n", infile);
        return 2;
    }
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not open '%s'.\n", outfile);
        return 3;
    }

    // Setup scaler and run validation
    float scaleF = atof(argv[1]);
    if (scaleF > 100 || scaleF <= 0)
    {
        fclose(inptr);
        fclose(outptr);
        fprintf(stderr, "Usage: %s <floating point (0.0, 100.0]> <infile> <outfile>\n", argv[1]);
        return 4;
    }

    // Setup BMP headers
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Validate BMP infile to be resized is 24-bit uncompressed
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(inptr);
        fclose(outptr);
        fprintf(stderr, "'%s' is not in a recognized format.\n", infile);
        return 5;
    }

    // determine resized dimensions (int truncates fractional part)
    // deal with edge cases where new dimensions would be < 1 and truncated to 0
    int newHeight = scaleF * abs(bi.biHeight);
    if (newHeight == 0)
    {
        newHeight = 1;
    }
    int newWidth = scaleF * abs(bi.biWidth);
    if (newWidth == 0)
    {
        newWidth = 1;
    }

    //Determine new file scanline padding
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //Update header values to be correct for output file
    bi.biWidth = (LONG) newWidth;
    bi.biHeight = (LONG) newHeight;
    bf.bfSize = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER)
        + ((newWidth * (sizeof(RGBTRIPLE) + sizeof(WORD) * padding))
        * newHeight);
    printf("%i\n", bf.bfSize);
}