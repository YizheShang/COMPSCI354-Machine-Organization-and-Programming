///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Spring 2021, Deb Deppeler
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        myMagicSquare.c
// This File:        myMagicSquare.c
// Other Files:      (name of all other files if any)
// Semester:         CS 354 Spring 2021
// Instructor:       deppeler
// 
// Author:           Yizhe Shang
// Email:            yshang24@wisc.edu
// CS Login:         yshang
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
    int size;           // dimension of the square
    int **magic_square; // pointer to heap allocated magic square
} MagicSquare;

/* TODO:
 * Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 */
int getSize() {
    char input[10];
    int size = 0;

    // Prompt the user for the magic square's size.
    printf("Enter magic square's size (odd integer >=3)\n");
    fgets(input, 10, stdin);
    size = atoi(input); 
    
    // Check if the number is valid.
    if (size < 3){
	    if (size % 2 == 0){
	    	printf("Magic square size must be odd.\n");
	    } else {
		printf("Magic square size must be >= 3.\n");
	    }
    } else if (size > 3 && size % 2 == 0){
	    printf("Magic square size must be odd.\n");
    }

    return size;   
} 
   
/* TODO:
 * Makes a magic square of size n using the alternate 
 * Siamese magic square algorithm from assignment and 
 * returns a pointer to the completed MagicSquare struct.
 *
 * n the number of rows and columns
 */
MagicSquare *generateMagicSquare(int n) {
	MagicSquare *ms = malloc(sizeof(MagicSquare));
	ms -> size = n;
	// Create a dynamicially allocated 2D array for size n*n asthe board.
	int **board= malloc(sizeof(int*) * n);
	// check if error exists with memory allocation
	if(board == NULL){
		printf("Error in memory allocation!");
		exit(1);
	}


	for (int i = 0; i < n; i++){
		*(board + i) = malloc(sizeof(int) * n);
		// Check if error exits with memory allocation
		if (*(board+i) == NULL){
			printf("Error in memory allocation!");
                	exit(1);
		}
	}



	// Initialize the board with 0.
	for (int k = 0; k < n; k++){
		for (int l = 0; l < n; l++){
			*(*(board + k)+l) = 0;
		}
	}

	// Create the number which will be putted in the board.
	int num = 1;

	int i = n / 2;
	int j = n - 1;

	// Put numbers on the board.
	while (num <= n*n){
		*(*(board + i)+j) = num;
		i = i - 1;
		j = j + 1;
			
		if (i == -1 && j == n){
			i = 0;
			j = n - 2;
		} else if (i == -1 || j == n){
			if (i == -1){
			    i = n-1;
			}

			if (j == n){
			    j = 0;
			}
		} else if (*(*(board + i)+j) != 0){
			i = i + 1;
			j = j - 2;
		}
			
		num += 1;
	}
	ms -> magic_square = board;
	
	return ms;    
} 

/* TODO:  
 * Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * magic_square the magic square to write to a file
 * filename the name of the output file
 */
void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) {
	// Open the file
	FILE *ofp = fopen(filename, "w");

	// Check if error exits in opening file.
	if (ofp == NULL) {
		printf("Cannot open file for reading\n");
		exit(1);
	}
	fprintf(ofp, "%i", magic_square->size);
	fprintf(ofp, "\n");

	// Write the ouput into the file.
	for (int i = 0; i < magic_square->size; i++){
		for (int j = 0; j < magic_square->size; j++){
			if (j == 0){
				fprintf(ofp, "%i", *(*(magic_square->magic_square + i)+j));
			} else {
				fprintf(ofp, ",%i", *(*(magic_square->magic_square + i)+j));
			}
		}
		fprintf(ofp, "\n");
	}
	
	fclose(ofp);
	//Close the input file
        if (fclose(ofp) != 0) {
		printf("Error while closing the input file\n");
		exit(1);
	}
}


/* TODO:
 * Generates a magic square of the user specified size and
 * output the quare to the output filename
 */
int main(int argc, char *argv[]) {
    // TODO: Check input arguments to get output filename
	if (argc != 2){
	    printf("Usage: ./myMagicSquare <output_filename>\n");
	    exit(1);
    }
    // TODO: Get magin square's size from user
	MagicSquare *msptr = NULL;
	int size = getSize();

    // TODO: Generate the magic square
   	msptr = generateMagicSquare(size);
 	

    // TODO: Output the magic square.
    fileOutputMagicSquare(msptr, *(argv+1));

    // Free memory.
    for(int i = 0; i < size; i++){
        free(*(msptr->magic_square + i));
    }

    free(msptr->magic_square);
    free(msptr);
    msptr = NULL;

    return 0;
} 






                                                         
//		s21		myMagicSquare.c      

