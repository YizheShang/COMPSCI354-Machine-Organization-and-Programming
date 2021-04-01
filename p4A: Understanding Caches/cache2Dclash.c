///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2021 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission Fall 2020, CS354-deppeler
//
///////////////////////////////////////////////////////////////////////////////
// Main File:        cache1D.c, cache2Drows.c, cache2Dcols.c and cache2Dclash.c
// This File:        cache2Dclash.c
// Other Files:      cache1D.c, cache2Drows.c, and cache2Dcols.c 
// Semester:         CS 354 Spring 2021
//
// Author:           Yizhe Shang
// Email:            yshang24@wisc.edu
// CS Login:         yshang
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   Avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////
int arr2D[128][8];

int main(){
    
    for (int iteration = 0; iteration < 100; iteration++){
	for (int row = 0; row < 128; row+=64){
            for (int col = 0; col < 8; col++){
		arr2D[row][col] = iteration + row + col;
	    }
	}
    }

    return 0;
}
