#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Function declarations
long long int mergeSort(int arr[], int left, int right); 
long long int merge(int arr[], int left, int mid, int right); 

int main(void) {
    
    printf("Enter the name of input file: ");
    char input_file[100];

    if (scanf("%s", input_file) != 1) {

        printf("Error: Failed to read input\n");
        return 1;
    }

    FILE *file = fopen(input_file, "r");
    if (file == NULL) {

        printf("Error: File not found\n");
        return 1;
    }

    // COunting number of lines in the file
    int no_of_lines = 0;
    char ch[4096];  

    while (fgets(ch, sizeof(ch), file) != NULL) {
        no_of_lines++;
    }
    fclose(file);

    //CHecking if the file is empty
    if (no_of_lines == 0) {
    printf("Error: Empty file\n");
    return 1;
    }

    //printf("No of lines in the file = %d\n", no_of_lines);

    int *values = (int *)malloc(no_of_lines * sizeof(int));

    if (values == NULL) {
        printf("Error: Memory allocation failed\n");
        return 1;
    }

    // Opening the file agian to read the values
    FILE *file1 = fopen(input_file, "r");
    if (file1 == NULL) {
        printf("Error: File not found\n");
        return 1;
    }

    int row = 0;
    
    while (fgets(ch, sizeof(ch), file1) != NULL) {
        values[row] = atoi(ch);  
        row++;
    }
    fclose(file1);

    // Counting inversions using merge sort
    long long int inversions = 0;
    inversions = mergeSort(values, 0, no_of_lines - 1);

    printf("Number of inversions = %lld\n", inversions);

    return 0;
}

// Merge function to merge two halves and count inversions
long long int merge(int arr[], int left, int mid, int right){
    int left_size = mid - left + 1;
    int right_size = right - mid;
    
    int *L = ( int *)malloc(left_size * sizeof(int));
    int *R = (int * )malloc(right_size * sizeof(int));

    if (L == NULL || R == NULL) {

        if (L) free(L);
        if (R) free(R);

        printf("Error: Memory allocation failed in merge\n");
        exit(1);
    }

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < left_size; i++){
        L[i] = arr[left + i];
    }

    for (int j = 0; j < right_size; j++){
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    long long int inv_count = 0;

    // Merging the temporary arrays back into arr[left..right]
    while (i < left_size && j < right_size){
        if (L[i] <= R[j]){
            arr[k] = L[i];
            i++;
        }
        else{
            arr[k] = R[j];
            j++;
            inv_count += (mid - left + 1 - i); // count inversions
        }
        k++;
    }

    // Copying the remaining elements of L[], if any
    while (i < left_size){
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copying the remaining elements of R[], if any
    while (j < right_size){
        arr[k] = R[j];
        j++;
        k++;
    }

    return inv_count;
}

// Merge sort function to sort the array and count inversions
long long int mergeSort(int arr[], int left, int right){

    long long int inv_count = 0;

    if (left < right){
        int mid = (left + right) / 2;

        // Recursively sorting first and second halves
        inv_count += mergeSort(arr, left, mid);
        inv_count += mergeSort(arr, mid + 1, right);

            // Merging the sorted halves and count inversions        
        inv_count += merge(arr, left, mid, right);
    }

    return inv_count;
}