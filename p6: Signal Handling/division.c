////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        division.c
// Other Files:      mySigHandler.c, sendsig.c
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
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

// Gloal variable that will count the number of successful division.
int counter = 0;

/* Handler when press control-c
 */
void handler_SIGINT() {
    	printf("\nTotal number of operations completed successfully: %d\n", counter);
    	printf("The program will be terminated.\n");
	exit(0);
}

/* Handler when divided by 0.
 */
void handler_SIGFPE() {
    printf("Error: a division by 0 operation was attempted.\n");
    printf("Total number of operations completed successfully: %d\n", counter);
    printf("The program will be terminated.\n");
    exit(1);
}

/*The main function that will run the code.
 *
 */
int main(){

	// Set up sigaction for SIGINT.
	struct sigaction act_INT;
    	memset(&act_INT, 0, sizeof(act_INT));
    	act_INT.sa_handler = handler_SIGINT;

	if (sigaction(SIGINT, &act_INT, NULL) != 0){
        	printf("ERROR in seeting SIGINT.");
        	exit(0);
    	}

	// Set up sigaction for SIGFPE.
	struct sigaction act_FPE;
    	memset(&act_FPE, 0, sizeof(act_FPE));
    	act_FPE.sa_handler = handler_SIGFPE;

	if (sigaction(SIGFPE, &act_FPE, NULL) != 0){
        	printf("ERROR in seeting SIGFPE.");
        	exit(0);
        }

	// An infinite loop that will prompt the user for two integer values.
	 while (1) {
		// Prompt for and read in the first integer value.
		printf("Enter first integer: ");
		char buf1[100];
    		fgets(buf1, 100, stdin);

		// Check the return value of fgets().
		if (buf1 == NULL){
			printf("Error in getting user input.");
			exit(1);
		}

		// Check if the first integer value is valid; if not, assign 
		// the first intager 0.
		int num1 = 0;
		int i = 0;
		while (i > 0){
            		if (isdigit(buf1[i]) == 0){
				i += 1;	
			}
		}
		if (i == 0){
			num1 = atoi(buf1);
		} 

		// Prompt for and read in the second integer value.
		printf("Enter second integer: ");
		char buf2[100];
       		fgets(buf2, 100, stdin);

		// Check the return value of fgets().
                if (buf2 == NULL){
                        printf("Error in getting user input.");
                        exit(1);
                }


		// Check if the second integer value is valid; if not, assign
	        // the second integer 0.
		int num2 = 0;
	        int j = 0;
	        while (j > 0){
			if (isdigit(buf2[j]) == 0){
                	        j += 1;
           	     	}
	        }
	        if (j == 0){
        	        num2 = atoi(buf2);
        	}

		// Calculate the quotient and remainde.
		int quotient = num1 / num2;
		int remainder = num1 - (num2 * quotient);

		// Print the results.
		printf("%d / %d is %d with a remainder of %d\n", num1, num2, quotient, remainder);

		// Increase the counter;
		counter++;
	 }



	return 0;
}
