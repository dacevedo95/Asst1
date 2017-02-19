#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>



/*Defines the Node struct which will hold metadata. The short size
indicates the size of the user data that the Node manages. The char state
is 0 if the memory is not being used. It is 1 if it is being used
*/
typedef struct NodeStruct {
	short size;
	char state;
} Node;

//intializes the char array to be used to simulate dynamic memory
static char myblock[5000];

//boolean to determine wether myblock has been intialized
static bool myblockIntialized=false;

//Initializes the memory block
void intializeMyBlock () {

	//Pointer to the start of the block
	Node * head=(Node *)myblock;

	//Create a node for all the unused space
	Node metadataHead={(5000-(short)sizeof(Node)),0};
	*head=metadataHead;
}

void * mymalloc(size_t size, char* file, int line) {

	/*If the size is > 5000-(the size of a Node), return NULL and
	print an informative error message
	*/ 
	if (size>5000-sizeof(Node))
	{
		printf("Error requesting memory at %s, line %d. The size requested is too large.\n",file ,line);
		return NULL;
	}

	//Initialize memory block if not already done
	if (myblockIntialized==false) {
		intializeMyBlock();
		myblockIntialized=true;
	}

	//Pointer to start of memory block
	Node * ptr=(Node *)myblock;

	/*Loop end if ptr exits the the 5000 bye memory block
	*/ 
	while(((char*)ptr-myblock)<5000) {

		//If the current block is in use, go to the next block
		if((*ptr).state==1) {
			ptr=(Node *)((char*)ptr+sizeof(Node)+(*ptr).size);
			continue;
		}

		short originalSize=(*ptr).size;

		/*If there is room to allocate memory of the inputted size and create
		a new metadata node with a size of at least 1
		*/
		if(originalSize>=(size+(short)sizeof(Node)+1)) {

			/*Update the size of ptr and set it as in use
			*/
			(*ptr).size=size;
			(*ptr).state=1;

			/*Create a new metadata node after ptr, set its size
			as the amount of memory remaming from the original block,
			and set its state to 0
			*/

			Node * next=(Node *)((char*)ptr+size+sizeof(Node));
			(*next).size=originalSize-size-(short)sizeof(Node);
			(*next).state=0;

			//Return ptr
			return (void *) ptr;
		} 

		/*If there is only enough room in the current block to allocate the inputted size,
		allocate all of the block and return ptr
		*/
		else if(originalSize>=size){
			(*ptr).state=1;
			return (void *) ptr;
		}
			
		
		ptr=(Node *)((char*)ptr+sizeof(Node)+(*ptr).size);
	}

		//If ptr exited the memory block then print an error message and return NULL
		printf("Error requesting memory at %s, line %d. Not enough memory available.\n",file ,line);
		return NULL;

}
