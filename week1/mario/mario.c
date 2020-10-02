#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;

    // Read input until it's correct
    do
    {
        height = get_int("Height: ");
    } 
    while (height < 1 || height > 8);

    
    for (int i = 0; i < height; i++) 
    {
        // Put spaces before the #
        for (int j = 0; j < (height - i - 1); j++) 
        {
            printf(" ");
        }
        
        // Put hashes before pyramids center
        for (int j = 0; j <= i; j++) 
        {
            printf("#");
        }
        
        // Pyramid center
        printf("  ");
        
        // Second half of pyramid
        for (int j = 0; j <= i; j++) 
        {
            printf("#");
        }
        
        printf("\n");
    }

}