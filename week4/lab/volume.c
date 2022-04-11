// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// define datatype that represents bytes of data.
typedef uint8_t BYTE;

// define datatype that stores audio samples
typedef int16_t SAMPLE;

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // COPY HEADER

    BYTE header_buffer[HEADER_SIZE];

    // read header from input and save into buffer
    fread(&header_buffer, sizeof(BYTE), HEADER_SIZE, input);

    // write header from buffer into output
    fwrite(&header_buffer, sizeof(BYTE), HEADER_SIZE, output);


    // COPY SAMPLES (and change volume)
    // Note: read head is in correct position already
    SAMPLE sample_buffer;
    while (fread(&sample_buffer, sizeof(SAMPLE), 1, input))
    {
        //change volume of buffer
        sample_buffer *= factor;
        fwrite(&sample_buffer, sizeof(SAMPLE), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
