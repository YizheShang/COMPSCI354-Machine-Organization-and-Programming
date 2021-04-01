///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2021 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission Fall 2020, CS354-deppeler
//
///////////////////////////////////////////////////////////////////////////////
// Main File:        myHeap.c
// This File:        myHeap.c
// Other Files:      myHeap.h
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


// DEB'S PARTIAL SOLUTION FOR SPRING 2021 DO NOT SHARE
 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "myHeap.h"
 
/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {           

    int size_status;
    /*
     * Size of the block is always a multiple of 8.
     * Size is stored in all block headers and in free block footers.
     *
     * Status is stored only in headers using the two least significant bits.
     *   Bit0 => least significant bit, last bit
     *   Bit0 == 0 => free block
     *   Bit0 == 1 => allocated block
     *
     *   Bit1 => second last bit 
     *   Bit1 == 0 => previous block is free
     *   Bit1 == 1 => previous block is allocated
     * 
     * End Mark: 
     *  The end of the available memory is indicated using a size_status of 1.
     * 
     * Examples:
     * 
     * 1. Allocated block of size 24 bytes:
     *    Allocated Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 25
     *      If the previous block is allocated p-bit=1 size_status would be 27
     * 
     * 2. Free block of size 24 bytes:
     *    Free Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 24
     *      If the previous block is allocated p-bit=1 size_status would be 26
     *    Free Block Footer:
     *      size_status should be 24
     */
} blockHeader;         

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */
blockHeader *heapStart = NULL;     

/* Size of heap allocation padded to round to nearest page size.
 */
int allocsize;

/*
 * Additional global variables may be added as needed below
 */

 
/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block (payload) on success.
 * Returns NULL on failure.
 *
 * This function must:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8 
 *   and possibly adding padding as a result.
 *
 * - Use BEST-FIT PLACEMENT POLICY to chose a free block
 *
 * - If the BEST-FIT block that is found is exact size match
 *   - 1. Update all heap blocks as needed for any affected blocks
 *   - 2. Return the address of the allocated block payload
 *
 * - If the BEST-FIT block that is found is large enough to split 
 *   - 1. SPLIT the free block into two valid heap blocks:
 *         1. an allocated block
 *         2. a free block
 *         NOTE: both blocks must meet heap block requirements 
 *       - Update all heap block header(s) and footer(s) 
 *              as needed for any affected blocks.
 *   - 2. Return the address of the allocated block payload
 *
 * - If a BEST-FIT block found is NOT found, return NULL
 *   Return NULL unable to find and allocate block for desired size
 *
 * Note: payload address that is returned is NOT the address of the
 *       block header.  It is the address of the start of the 
 *       available memory for the requesterr.
 *
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* myAlloc(int size) {     
    //TODO: Your code goes in here.
    // Check if the size is positive.
    if (size <= 0){
	    return NULL;
    }

  //  if (size > 

    // Make the block size be the multiple of 8.
    int block_size, padding;
    if (size + sizeof(blockHeader) <= 8){
	block_size = 8;
//	padding = 8 - (size + sizeof(blockHeader));
    } else {	
	int remainder = (size + sizeof(blockHeader)) % 8;
	if (remainder == 0){
	    padding = 0;
	} else {
	    padding = 8 - remainder;
	}
        block_size = sizeof(blockHeader) + size + padding;
    }

  //  printf("%i \n", block_size);
     
    blockHeader *current = heapStart;
    blockHeader *most_suitable = NULL; 

    // Traverse memory blocks
    while (current->size_status != 1){
	int current_size = current->size_status - ((current->size_status) & 3);

	// Check if the current block is allocated.    
	if ((current->size_status & 1) == 1) {

		// Skip to the next block;
		current = (blockHeader*)((void*)current + current_size);
	} else {
		// Check if the current block has the same size.
		if (current_size ==  block_size){
		        
			// Update the header.
			current->size_status += 1;
			
			blockHeader *next = (blockHeader*) ((void*)current + current_size);
			// Change the p-bit in the header of the next block if the next block is not the heap end.
			if ((next->size_status != 1) && (next->size_status & 2) != 2){
				next->size_status += 2;
			}

		// Return the pointer of the new allocated block.
		return ((void*)current + sizeof(blockHeader));
		} else {

		        // Check if the block size of the current block is smaller than the block size.
			if (current_size < block_size){

				// If it is, skip to the next block;
				current = (blockHeader*) ((void*)current + current_size);
			} else {
				
		       		// Check if we have a possible suitable block; if we do not, assign one. 
				if (most_suitable == NULL){
					most_suitable = current;
				} else{

					int most_suitable_size = most_suitable->size_status - ((most_suitable->size_status)&3); 
			
					// Check if the current blcok has a smaller block size than the suitable block we have.
					if (most_suitable_size > current_size){
						most_suitable = current;
					}	
				}		
			
				// Skip to the next block;
                        	current = (blockHeader*) ((void*)current + current_size);
		
			}	
		}	
    	}	
    }


      // Check if a free block with larger block size is found; if it is not, return NULL.
    if (most_suitable == NULL){
        return NULL;
    } else {

        // A free block with larger block size is found.
        // Get the larger block size.
        int big_block_size = most_suitable->size_status - ((most_suitable->size_status) & 3);

        // Get the rest block size after minusing the block_size.
        int rest_block_size = big_block_size - block_size;

        // Allcate the free block with proper size.
        most_suitable->size_status = block_size + (most_suitable->size_status & 2) + 1;

        // Update the footer of the new free block.
        blockHeader *rest = (blockHeader*)((void*)most_suitable + block_size);
	
//	printf("%p \n", rest);
//	printf("%p \n,", most_suitable);

        // Update the header of the new free block;
        rest->size_status = rest_block_size + 2;

        // Return the pointer of the new allocated block.
        return ((void*)most_suitable + sizeof(blockHeader));


    }

   
} 
 
/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - Update header(s) and footer as needed.
 */                    
int myFree(void *ptr) {    

	// If ptr is NULL, return -1;
    if (ptr == NULL){
            return -1;
    }

    // If ptr is not a multiple of 8, return -1.
    if ((int)ptr % 8 != 0){
            return -1;
    }

    // Get the total heap size.
    int heap_size = 0;
    
    blockHeader *current = heapStart;
    while (current->size_status != 1){

            // Calculate the total heap size.
            heap_size += (current->size_status - (current->size_status & 3));
            current = (blockHeader*)((void*)current + current->size_status - (current->size_status & 3));
    }
    
    
    if ((int)ptr < ((int)(void*)heapStart) || (int)ptr >= (int)((void*)heapStart + heap_size)){
            return -1;
    }

    // Get the block_header of ptr.
    blockHeader *block = (blockHeader*)((void*)ptr - sizeof(blockHeader));

    // If ptre block is freed, return -1.
    if ((block->size_status & 1) == 0){
	return -1;
    }

    // Free the block.
    block->size_status -= 1;

    // Get the new block size.
    int block_size = block->size_status - (block->size_status & 3);

    // Update the footer.
    blockHeader *footer = (blockHeader*)((void*)block + block_size - sizeof(blockHeader));
    footer->size_status = block_size;

    // Get the next block and check if the next block is the end mark.
    blockHeader *next = (blockHeader*)((void*)block + block_size);
    if (next->size_status != 1){

            // Update the header of the next block.
            next->size_status -= 2;
    }

    return 0;

 } 

/*
 * Function for traversing heap block list and coalescing all adjacent 
 * free blocks.
 *
 * This function is used for delayed coalescing.
 * Updated header size_status and footer size_status as needed.
 */
int coalesce() {
   
    int num_coalesce = 0;	
    blockHeader *current = heapStart;
    blockHeader *next = NULL;

    while (current->size_status != 1){
	
	// Get the size of current heap.                
        int current_size = current->size_status - (current->size_status & 3);

           
       	// Check if the current block is allocated.
        if ((current->size_status & 1) == 1){

                // If it is, skip to the nect block.
                current = (blockHeader*) ((void*)current + current_size);
        } else {

		// If it is not, get the next block.
                next = (blockHeader*) ((void*)current + current_size);


                // Check if the next block is also free.
                if ((next->size_status & 1) == 0){

                	// Get the size of the next block.
                	int next_size = next->size_status - (next->size_status & 3);
			
	        	// Update header.
       	        	current->size_status += next_size;

			// Get the footer.
		
			blockHeader *footer = (blockHeader*)((void*)current + next_size - sizeof(blockHeader));
		        // Update ther footer.
			footer->size_status = current_size + next_size;

			num_coalesce += 1;
                } else {
                        current = next;
                }
            }
    }

    // Returns 0 if there were no adjacent free blocks coalesced; otherwise return a positive numberoi-=.
    if (num_coalesce != 0){
	return num_coalesce;
    } else {
	return 0;
    }
}

 
/* 
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int myInit(int sizeOfRegion) {    
 
    static int allocated_once = 0; //prevent multiple myInit calls
 
    int pagesize;   // page size
    int padsize;    // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int fd;

    blockHeader* endMark;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }

    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    allocsize = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, allocsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
    allocated_once = 1;

    // for double word alignment and end mark
    allocsize -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heapStart = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    endMark = (blockHeader*)((void*)heapStart + allocsize);
    endMark->size_status = 1;

    // Set size in header
    heapStart->size_status = allocsize;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heapStart->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heapStart + allocsize - 4);
    footer->size_status = allocsize;
  
    return 0;
} 
                  
/* 
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void dispMem() {     
 
    int counter;
    char status[6];
    char p_status[6];
    char *t_begin = NULL;
    char *t_end   = NULL;
    int t_size;

    blockHeader *current = heapStart;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int is_used   = -1;

    fprintf(stdout, 
	"*********************************** Block List **********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, 
	"---------------------------------------------------------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "alloc");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "FREE ");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "alloc");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "FREE ");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%4i\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, 
	"---------------------------------------------------------------------------------\n");
    fprintf(stdout, 
	"*********************************************************************************\n");
    fprintf(stdout, "Total used size = %4d\n", used_size);
    fprintf(stdout, "Total free size = %4d\n", free_size);
    fprintf(stdout, "Total size      = %4d\n", used_size + free_size);
    fprintf(stdout, 
	"*********************************************************************************\n");
    fflush(stdout);

    return;  
} 


// end of myHeap.c (sp 2021)                                         

