////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        mySigHandler.c
// Other Files:      division.c, sendsig.c
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

// The time interval.
int seconds = 3; 
// A global counter used to keep tally of the number of times it receives SIGUSR1.
int counter = 0;

/**
 * Initialize the sigaction struct of SIGALRM.
 * */
void handler_SIGALRM(){


	// Obtain the pid of the program.
        pid_t pid = getpid();

	// obtain the time in time_t.
	time_t process_time = time(NULL);
	// Obtain the time in String format.
	char *time_str = ctime(&process_time);

	// Check if the time is valid.
	if (process_time < 0 || time_str == NULL){
		printf("Error occured in calculating time.");
		exit(1);
	}

	// Print the message. 
	printf("PID: %d CURRENT TIME: %s", pid, time_str);

	alarm(seconds);
}


/**
 * Initialize the sigaction struct of SIGUSR1.
 * */

void handler_SIGUSR1(){
	// Print the message.
	printf("SIGUSR1 handled and counted!\n");

	// Increase the counter;
    	counter+=1;
}

void handler_SIGUSR2(){
}


/**
 * Initialize the sigaction struct of SIGINT.
 * */
void handler_SIGINT() {

	// Print the messgae.
    	printf("\nSIGINT handled.\nSIGUSR1 was handled %d times. Exiting now.\n", counter);
    	exit(0);
}

/**
 *Ths main function that will run the code.
 * */
int main(){

	// Print thr message.
	printf("PID and time print every 3 seconds.\n");
	printf("Type Ctrl-C to end the program.\n");

	// Set up sigaction for SIGALRM
    	struct sigaction act;
	// Initialize the sigaction struct.   
	memset(&act, 0, sizeof(act));

	// Triger the first SIGALRM.
	alarm(seconds); 

	act.sa_handler = handler_SIGALRM;

    	// Handle SIGALRM signal.
    	if (sigaction(SIGALRM, &act, NULL) != 0){
        	printf("ERROR in registering sigalarm\n");
        	exit(1);
    	}	

	// Set up sigaction for SIGUSR1
   	struct sigaction act_SIGUSR1;
	// Initialize the sigaction struct.
    	memset(&act_SIGUSR1, 0, sizeof(act_SIGUSR1));

	act_SIGUSR1.sa_handler = handler_SIGUSR1;

	// Handle SIGUSR1 signal.
    	if (sigaction(SIGUSR1, &act_SIGUSR1, NULL) != 0){
        	printf("ERROR in registering sigusr1\n");
        	exit(1);
    	}

	// Set up sigaction for SIGINT
    	struct sigaction act_SIGINT;
    	// Initialize the sigaction struct.
	memset(&act_SIGINT, 0, sizeof(act_SIGINT));

    	act_SIGINT.sa_handler = handler_SIGINT;

	// Handle SIGINT signal.
    	if (sigaction(SIGINT, &act_SIGINT, NULL) != 0){
        	printf("ERROR in registering sigint\n");
        	exit(1);
    	}

    	// infinite loop
    	while (1) {
    	}


	return 0;
}

