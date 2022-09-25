#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// define byte
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // ensure correct command-line useage
    if (argc != 2)
    {
        printf("Useage: ./recover memorycard\n");
        return 1;
    }

    // open memory card 
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)                    
    {
        fprintf(stderr, "Could not open file");
        return 2;
    }

    // buffer for file names
    char output[8];
    
    // keep track of file number for file names
    int counter = 0; 
    
    // pointer to write JPEGs to
    FILE *outptr = NULL;
    
    // read into infile, 512 bytes at a time until end of file
    BYTE buffer[512];
    while (fread(buffer, sizeof(BYTE), 512, inptr) == 512)
    {
        // New JPEG signatiture found
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // if not first JPEG, close previous output file
            if (outptr != NULL)
                fclose(outptr);
            
            // write new file name
            sprintf(output, "%03i.jpg", counter);
            
            // open new file
            outptr = fopen(output, "w");
            if (outptr == NULL)
            {
                fprintf(stderr, "Could not open file.\n");
                fclose(inptr);
                return 4;
            }
            
            // increment file counter
            counter++;
        }

        // if output file is already open
        if (outptr != NULL)
        {
            // keep writing into JPEG
            fwrite (buffer, sizeof(BYTE), 512, outptr);
        }
    }
    
    // close final file if still open
    if (outptr != NULL)
        fclose(inptr);
}