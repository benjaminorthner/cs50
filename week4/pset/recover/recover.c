#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

// define filetype BYTE
typedef uint8_t  BYTE;

// Define the size of one block in bytes
const int BLOCKSIZE = 512;

int main(int argc, char *argv[])
{
    // ensure exactly 1 command line argument is given
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover <infile>\n");
        return 1;
    }


    // open file containing raw data
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s\n", argv[1]);
        return 2;
    }

    //init buffer and total raw data size counter
    // buffer holds one block, i.e. 512 Bytes
    BYTE buffer[BLOCKSIZE];
    size_t blocksRead = 0;

    int jpgCount = -1;

    // read one block each cycle and terminate when fread returns 0 (end of file is reached)
    while ((blocksRead = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        // allocate memory for name of jpg
        char *jpgName = (char *)malloc(8 * sizeof(char));

        // recognise start of new jpg
        // note bitwise & 0xf0 is the same as bitwise & 11110000 and thus only looks at first 4 bits which are 0xe
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            jpgCount++;
        }

        // check if first jpg has been found (if not then throw away data and move to next block)
        if (!(jpgCount < 0))
        {
            // update name for jpg
            sprintf(jpgName, "%03i.jpg", jpgCount);

            // open jpg File if doesn't exist (if it does exist then append to it)
            FILE *jpgFile = fopen(jpgName, "a");

            // write current buffer to jpg
            fwrite(buffer, sizeof(buffer), 1, jpgFile);
            fclose(jpgFile);
        }
        free(jpgName);
    }

    return 0;
}