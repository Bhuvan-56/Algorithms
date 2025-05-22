#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_SIZE 2010 // Maximum size for input strings


// FUnction to find the minimum value among a, b, and c
int mini(int a, int b, int c) {
    if((a <= b) && (a <= c)) {
        return a;
    } else if((b <= a) && (b <= c)) {
        return b;
    } else {
        return c;
    }
}


// Calculates Edi distance between two strings using dynamic programming
int editDistance(char *str1, char *str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    
    // Allocate memory for the dynamic programming table
    int **dp = (int **)malloc((len1 + 1) * sizeof(int *));
    for (int i = 0; i <= len1; i++) {
        dp[i] = (int *)malloc((len2 + 1) * sizeof(int));
    }
    
    // Initialize the first row and column of the DP table

    for (int i = 0; i <= len1; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= len2; j++) {
        dp[0][j] = j;
    }
    

    // Fill the DP table using the recurrence relation
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int pos1 = dp[i-1][j] + 1; // Cost of Deletion
            int pos2 = dp[i][j-1] + 1; // Cost of Insertion
            int pos3 = dp[i-1][j-1]; // Cost if no operation is needed
            if (str1[i-1] != str2[j-1]) {
                pos3 += 1;
            }

            // We consider the minimum cost of three operations
            dp[i][j] = mini(pos1, pos2, pos3);
        }
    }
    
    // The final result is in the bottom-right cell of the DP table
    int result = dp[len1][len2];

    // printf("DP Table:\n");
    // printf("    ");
    // for (int j = 0; j <= len2; j++) {
    //     if (j == 0) printf("  ");
    //     else printf("%c ", str2[j-1]);
    // }
    // printf("\n");

    // for (int i = 0; i <= len1; i++) {
    //     if (i == 0) printf("  ");
    //     else printf("%c ", str1[i-1]);
        
    //     for (int j = 0; j <= len2; j++) {
    //         printf("%d ", dp[i][j]);
    //     }
    //     printf("\n");
    // }
        

    for (int i = 0; i <= len1; i++) {
        free(dp[i]);
    }
    free(dp);
    
    return result;
}

// main to read input files and calculate Edit Distance
int main(void) {
    printf("Enter the name of the first file: ");
    char filename1[100];
    if (scanf("%s", filename1) != 1) {
        printf("Error reading filename1\n");
        return 1;
    }

    printf("Enter the name of the second file: ");
    char filename2[100];
    if ( scanf("%s", filename2) != 1) {
        printf("Error reading filename2\n");
        return 1;
    }

    FILE *file1;
    FILE *file2;

    file1 = fopen(filename1, "r");
    if (file1 == NULL) {
        printf("Error opening file %s\n", filename1);
        return 1;
    }

    char *str1 = (char *)malloc(MAX_SIZE * sizeof(char));
    char *str2 = (char *)malloc(MAX_SIZE * sizeof(char));

    if (str1 == NULL) {
        printf("Memory allocation failed\n");
        fclose(file1);
        return 1;
    }

    file2 = fopen(filename2, "r");
    if (file2 == NULL) {
        printf("Error opening file %s\n", filename2);
        free(str1);
        fclose(file1);
        return 1;
    }

    if (str2 == NULL) {
        printf("Memory allocation failed\n");
        free(str1);
        fclose(file1);
        fclose(file2);
        return 1;
    }

    // Read and process the first file
    // Ignoring whitespace characters and storing only meaningful text

    int i = 0;
    int ch1;
    while ((ch1 = fgetc(file1)) != EOF) {
        if (ch1 != ' ' && ch1 != '\n' && ch1 != '\t' && ch1 != '\r') {
            str1[i++] = ch1;
            if (i >= MAX_SIZE - 1) break;
        }
    }
    str1[i] = '\0'; // Null teerminate the string

    
    int j = 0;
    int ch2;
    while ((ch2 = fgetc(file2)) != EOF) {
        if (ch2 != ' ' && ch2 != '\n' && ch2 != '\t' && ch2 != '\r') {
            str2[j++] = ch2;
            if (j >= MAX_SIZE - 1) break;
        }
    }
    str2[j] = '\0'; // Null terminate the string

    // printf("String 1 after processing: %s (length: %zu)\n", str1, strlen(str1));
    // printf("String 2 after processing: %s (length: %zu)\n", str2, strlen(str2));

    int distance = editDistance(str1, str2);
    
    // printf("String 1: %s\n", str1);
    // printf("String 2: %s\n", str2);
    printf("Edit Distance for the files %s and %s : %d\n",filename1,filename2, distance);


    free(str1);
    free(str2);
    fclose(file1);
    fclose(file2);

    return 0;
}