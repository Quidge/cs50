#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
// I could include greatest common factor as a module, but I don't think it would pass submit50

int gcf(int a, int b);

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
    BITMAPFILEHEADER in_bf;
    fread(&in_bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    BITMAPINFOHEADER in_bi;
    fread(&in_bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Validate BMP infile to be resized is 24-bit uncompressed
    if (in_bf.bfType != 0x4d42 || in_bf.bfOffBits != 54 || in_bi.biSize != 40 ||
        in_bi.biBitCount != 24 || in_bi.biCompression != 0)
    {
        fclose(inptr);
        fclose(outptr);
        fprintf(stderr, "'%s' is not in a recognized format.\n", infile);
        return 5;
    }

    // Setup outfile headers
    BITMAPFILEHEADER out_bf = in_bf;
    BITMAPINFOHEADER out_bi = in_bi;

    // determine resized dimensions (int truncates fractional part, this isn't great)
    // deal with edge cases where new dimensions would be < 1 and truncated to 0
    int newHeight = scaleF * in_bi.biHeight;
    if (newHeight == 0)
    {
        newHeight = 1;
    }
    int newWidth = scaleF * in_bi.biWidth;
    if (newWidth == 0)
    {
        newWidth = 1;
    }

    printf("Scale: %f, old dimensions: %ix%i, new dimensions: %ix%i\n",
        scaleF, abs(in_bi.biHeight), in_bi.biWidth, newHeight, newWidth);

    // determine scanline padding for newfile
    int padding = (4 - (newWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //Update header values to be correct for output file
    out_bi.biWidth = (LONG) newWidth;
    out_bi.biHeight = (LONG) newHeight;
    // size in bytes of the image itself (+ padding bytes)
    out_bi.biSizeImage = ((newWidth * sizeof(RGBTRIPLE)) + padding) * abs(newHeight);
    // size in bytes of the image (bi.biSizeImage) + headers
    out_bf.bfSize = out_bi.biSizeImage + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);

    // Write new headers to resized file
    fwrite(&out_bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&out_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // idiot resize algo because i'm not smart
    // Idea: if scale factor is even integer, life is easy. Original pixel
    // represents #scale factor pixels.
    //
    // Ultimately, idea is this. Product of orig dimension (oD) and scaleF is
    // new dimension (nD).
    // > Get GCF of oD and nD
    // > oD / GCF = oDF and nD / GCF = nDF
    // The pixels composing oDF are responsible for what will compose
    // nDF. So for a 50x50 -> 30x30 resize, GCF is 10, oDF is 5, and
    // nDF is 3. So 5 pixels of the old image will be responsible
    // for 3 pixels of the new image.
    //
    // Here's where it gets stupid.
    //
    // My idea is this: overflow = oDF - nDF
    // Write (oDF - overflow) pixels to nDF pixel area (this will
    // inherently clip when oDF > nDF, and this is why it's stupid).
    // For nDF > oDF, it should actually work out fine.

    int aY = abs(in_bi.biHeight), aX = in_bi.biWidth;
    int bY = abs(out_bi.biHeight), bX = out_bi.biWidth;

    int gcfY = gcf(aY, bY), gcfX = gcf(aX, bX);

    int aYFactor = aY / gcfY, aXFactor = aX / gcfX;
    int bYFactor = bY / gcfY, bXFactor = bX / gcfX;

    int overflowY = aYFactor - bYFactor;
    int overflowX = aXFactor - bXFactor;

    // run through scanlines of infile
    for (int row = 0, height = abs(out_bi.biHeight); row < rowlen; row++)
    {
        for (int block = 0, len = gcfX; block < len; block++)
        {
            fread(&temp, sizeof)
        }
    }
    fclose(inptr);
    fclose(outptr);
}

int gcf(int a, int b)
{
  int c;
  while ( a != 0 ) {
     c = a; a = b%a;  b = c;
  }
  return b;
}