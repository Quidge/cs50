#include <cs50.h>
#include <stdio.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./whodunit infilename outfilename\n");
        return 1;
    }
    char *infile = argv[1];
    char *outfile = argv[2];

    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s\n", infile);
        return 2;
    }

    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not open %s\n", outfile);
        return 3;
    }

    //Assign file header.
    BITMAPFILEHEADER bFileH;
    fread(&bFileH, sizeof(BITMAPFILEHEADER), 1, inptr);

    //Assign info header.
    BITMAPINFOHEADER bInfoH;
    fread(&bInfoH, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Check for invalid format.
    if (bFileH.bfType != 0x4d42 || bFileH.bfOffBits != 54 ||
        bInfoH.biCompression != 0 || bInfoH.biSize != 40 ||
        bInfoH.biBitCount != 24)
    {
        fclose(inptr);
        fclose(outptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    // write new file headers
    fwrite(&bFileH, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bInfoH, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (bInfoH.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate through scan lines
    for (int row = 0, height = abs(bInfoH.biHeight); row < height; row++)
    {
        for (int px = 0; px < bInfoH.biWidth; px++)
        {
            RGBTRIPLE temp;
            fread(&temp, sizeof(RGBTRIPLE), 1, inptr);
            // remember that every write has 'seeked' to the latest pos
            // so no need to worry about that.
            if (temp.rgbtBlue == 0 && temp.rgbtGreen == 0 && temp.rgbtRed == 255)
            {
                // strategy is to change red pixels to white, all others to black
                temp.rgbtBlue = 255;
                temp.rgbtGreen = 255;
            }

            fwrite(&temp, sizeof(RGBTRIPLE), 1, outptr);
        }
        // add padding to end of line (if any)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    fclose(inptr);
    fclose(outptr);

    //success
    return 0;
}