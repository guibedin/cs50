#include <stdio.h>
#include <cs50.h>

#define SIZE 10

void merge_sort(int array[], int size);
void print_array(int array[], int size);

int main(void) {

    int array[SIZE] = {1, 0, 3, 5, 2, 4, 10, 8, 7, 6};

    //printf("Before:\n");
    print_array(array, SIZE);
    merge_sort(array, SIZE);
    /*
    printf("After: ");
    print_array(array);
    */
}

void merge_sort(int array[], int size) {

    if(size == 1) {
        //printf("LEN: 1");
        return;
    } else {
        int size_left = 0;
        int size_right = 0;

        // if array size is even, split array in 2 equal parts
        if(size % 2 == 0) {
            size_left = (size / 2);
            size_right = (size / 2);
        } else { // if it's odd, left size will be bigger
            size_left = (size / 2) + 1;
            size_right = (size / 2);
        }

        //printf("SIZE LEFT: %i, SIZE RIGHT: %i\n", size_left, size_right);
        int left_array[size_left];
        int right_array[size_right];

        for(int i = 0; i < size_left; i++) {
            left_array[i] = array[i];
        }
        for(int i = 0; i < size_right; i++) {
            right_array[i] = array[size_left + i];
        }

        printf("\nArray: ");
        print_array(array, size);
        printf("Left: ");
        print_array(left_array, size_left);
        printf("Right: ");
        print_array(right_array, size_right);

        merge_sort(left_array, size_left);
        merge_sort(right_array, size_right);

        return;
    }
}

void print_array(int array[], int size) {

    printf("Size: %i - Array: [ ", size);
    for(int i = 0; i < size; i++) {
        printf("%i ", array[i]);
    }
    printf("]\n");
}