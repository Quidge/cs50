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

    // determine resized dimensions (int truncates fractional part, this isn't great)
    // deal with edge cases where new dimensions would be < 1 and truncated to 0
    int newHeight = scaleF * bi.biHeight;
    if (newHeight == 0)
    {
        newHeight = 1;
    }
    int newWidth = scaleF * bi.biWidth;
    if (newWidth == 0)
    {
        newWidth = 1;
    }

    printf("Scale: %f, old dimensions: %ix%i, new dimensions: %ix%i\n",
        scaleF, abs(bi.biHeight), bi.biWidth, newHeight, newWidth);

    // determine scanline padding for newfile
    int padding = (4 - (newWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //Update header values to be correct for output file
    bi.biWidth = (LONG) newWidth;
    bi.biHeight = (LONG) newHeight;
    bf.bfSize = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER)
        + ((newWidth * (sizeof(RGBTRIPLE) + sizeof(WORD) * padding))
        * newHeight);
    bi.biSizeImage = (newWidth + padding) * abs(newHeight);

    // Write new headers to resized file
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate through scanlines from oldFile
    for (int i = 0; i < abs(newHeight); i++)
    {
        // iterate through pixels on each scaline
        for (int j = 0; j < newWidth; j++)
        {
            // lol so i'm just going to write over every pixel with black
            RGBTRIPLE temp = {0x00, 0x00, 0x00};
            fwrite(&temp, sizeof(RGBTRIPLE), 1, outptr);
        }

        for (int j = 0; j < padding; j++)
        {
            fputc(0x00, outptr);
        }
    }
    fclose(inptr);
    fclose(outptr);
}