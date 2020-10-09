#include <stdio.h>
#include <cs50.h>

#define SIZE 9

void bubble_sort(int array[], int size);
void print_array(int array[], int size);

int main(void) {

    int array[SIZE] = {1, 3, 5, 2, 4, 10, 8, 7, 6};

    printf("Before: ");
    print_array(array, SIZE);

    bubble_sort(array, SIZE);

    printf("After: ");
    print_array(array, SIZE);
}

void bubble_sort(int array[], int size) {

    int sorted = 0;
    while(sorted == 0) {
        sorted = 1;
        for(int i = 0; i < size - 1; i++) {
            if(array[i] > array[i + 1]) {
                int temp = array[i + 1];
                array[i + 1] = array[i];
                array[i] = temp;
                sorted = 0;
            }
        }
    }
}

void print_array(int array[], int size) {

    printf("Size: %i - Array: [ ", size);
    for(int i = 0; i < size; i++) {
        printf("%i ", array[i]);
    }
    printf("]\n");
}