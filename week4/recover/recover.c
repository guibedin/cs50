#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Usage: ./recover infile\n");
        return 1;
    }

    // Open infile
    FILE *card = fopen(argv[1], "r");

    // check if correctly opened
    if(card == NULL) {
        printf("Could not open file %s.\n", argv[1]);
        return 2;
    }

    // allocates a block of memory - 512 bytes
    unsigned char *block = malloc(BLOCK_SIZE);

    // error check for mallloc
    if(block == NULL) {
        fclose(card);
        return 2;
    }
    int jpg_counter = 0;

    // Reads the file 1 block at a time
    while(fread(block, BLOCK_SIZE, 1, card)) {

        // Found a jpeg! -- Write it to ###.jpg
        if(block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && block[3] >= 0xe0 && block[4] < 0xf0) {

            // filename will always be ###.jpg = 7 + 1 (\0) = 8
            char *filename = malloc(8);
            if(filename == NULL) {
                free(block);
                fclose(card);
                return 3;
            }

            // formats filename to ###.jpg
            sprintf(filename, "%03i.jpg", jpg_counter);

            // Open jpg file to write
            FILE *jpg = fopen(filename, "w");
            if(jpg == NULL) {
                free(block);
                fclose(card);
                free(filename);
                return 4;
            }

            // Write first block of jpg file
            fwrite(block, BLOCK_SIZE, 1, jpg);

            // Reads next blocks until finding the next jpg
            while(fread(block, BLOCK_SIZE, 1, card)) {
                if(block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && block[3] >= 0xe0 && block[4] < 0xf0) {
                    // Go back 1 block with the "card" file cursor, finished the jpg and will start another
                    fseek(card, -BLOCK_SIZE, SEEK_CUR);
                    jpg_counter++;
                    break;
                }
                fwrite(block, BLOCK_SIZE, 1, jpg);
            }
            // close jpg file and free filename string
            fclose(jpg);
            free(filename);
        }
    }
    //printf("Found %i jpegs!\n", jpg_counter);
    fclose(card);
    free(block);

    return 0;
}
