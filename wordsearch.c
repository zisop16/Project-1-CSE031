#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
char* visits(int** searchPaths, int row, int col, int wordSize, int paths);
int* recursiveSearchPuzzle(char** arr, char* word, int row, int col, int targetInd);
int bSize;

// Main function, DO NOT MODIFY 	
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);
    
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);            
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%s", word);
    
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    
    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
    
    return 0;
}

void printPuzzle(char** arr) {
	for (int i = 0; i < bSize; i++) {
        char* row = *(arr + i);
        for (int j = 0; j < bSize; j++) {
            char curr = *(row + j);
            printf("%c", curr);
            if (j != bSize - 1) {
                printf("%c", ' ');
            }
        }
        printf("%c", '\n');
    }
}

void searchPuzzle(char** arr, char* word) {
    // This function checks if arr contains the search word. If the 
    // word appears in arr, it will print out a message and the path 
    // as shown in the sample runs. If not found, it will print a 
    // different message as shown in the sample runs.
    // Your implementation here...

    int i = 0;
    while (*(word + i) != '\0') {
        char curr = *(word + i);
        if (curr >= 'a') {
            *(word + i) = curr + 'A' - 'a';
        }
        i++;
    }
    int wordSize = i;
    int** searchPaths = malloc(sizeof(int*) * bSize * bSize);
    int viablePaths = 0;
    for (int r = 0; r < bSize; r++) {
        for (int c = 0; c < bSize; c++) {
            char curr = *(*(arr + r) + c);
            int* searchPath;
            if (curr == *word) {
                searchPath = recursiveSearchPuzzle(arr, word, r, c, 1);
            }
            if (searchPath != NULL) {
                *(searchPath) = r;
                *(searchPath + 1) = c;
                *(searchPaths + viablePaths) = searchPath;
                viablePaths++;
            }
        }
    }
    printf("%c", '\n');
    if (viablePaths == 0) {
        printf("Word not found!");
        return;
    }
    printf("Printing the search path:\n");
    int spacing = 8;
    for (int r = 0; r < bSize; r++) {
        for (int c = 0; c < bSize; c++) {
            char* visitsAtPosition = visits(searchPaths, r, c, wordSize, viablePaths);
            printf(visitsAtPosition);
            for (int i = 0; i < spacing - strlen(visitsAtPosition); i++) {
                printf("%c", ' ');
            }
        }
        printf("%c", '\n');
    }

}

char* visits(int** searchPaths, int row, int col, int wordSize, int paths) {
    // This is not guaranteed to be enough memory,
    // But I don't care
    char* toReturn = malloc(sizeof(char) * wordSize * 10);
    for (int i = 0; i < paths; i++) {
        int* path = *(searchPaths + i);
        int numVisits = 0;
        for (int j = 0; j < wordSize; j++) {
            int r = *(path + j * 2);
            int c = *(path + j * 2 + 1);
            if (r == row && c == col) {
                *(toReturn + numVisits) = j + '1';
                numVisits++;
            }
        }
        // We assume paths dont intersect, so we just return the visits from a single path
        if (numVisits != 0) {
            return toReturn;
        }
    }
    return "0";
}

int* recursiveSearchPuzzle(char** arr, char* word, int row, int col, int targetInd) {
    // Prev represents the index of the word
    // targetInd = index of character in word we are looking at
    // If targetInd == 0 and word == HELLO, then target == H
    // If we reach the end of a chain, we return a new int* which recursively feeds back up

    int endOfString = *(word + targetInd + 1) == '\0';
    int* searchOrder;
    for (int r = row - 1; r <= row + 1; r++) {
        if (r == -1 || r == bSize) {
            continue;
        }
        char* currRow = *(arr + r);
        for (int c = col - 1; c <= col + 1; c++) {
            if (c == -1 || c == bSize) {
                continue;
            }
            if (c == col && r == row) {
                continue;
            }
            char target = *(word + targetInd);
            char curr = *(currRow + c);
            if (curr == target) {
                if (endOfString) {
                    searchOrder = malloc(sizeof(int) * 2 * (targetInd + 1));
                    *(searchOrder + 2 * targetInd) = r;
                    *(searchOrder + 2 * targetInd + 1) = c;
                    return searchOrder;
                }
                searchOrder = recursiveSearchPuzzle(arr, word, r, c, targetInd + 1);
                if (searchOrder != NULL) {
                    *(searchOrder + 2 * targetInd) = r;
                    *(searchOrder + 2 * targetInd + 1) = c;
                    return searchOrder;
                }
            }
        }
    }
    if (endOfString) {
        return NULL;
    }
}