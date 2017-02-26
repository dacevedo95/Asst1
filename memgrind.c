#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>

#include "mymalloc.h"

/* This function mallocs 1 byte 1000 times. Then it frees the pointers 1 by 1
*/
double workloadA() {

	struct timeval  tv1, tv2;

	//Start time
	gettimeofday(&tv1, NULL);

	//Array to hold the malloced pointers
	char * pointers[1000];

	//Make 1000 malloc calls of 1 byte
	for(int i=0;i<1000;i++) {
		pointers[i]=malloc(1);
	}

	//Free all the pointers
	for (int i = 0; i < 1000; ++i ){
		free(pointers[i]);
	}

	//Exit time
	gettimeofday(&tv2, NULL);

	//Return the time elapsed
	return (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);

}

/* This function mallocs 1 byte and then immediately frees it 1000 times.
*/
double workloadB() {
	struct timeval  tv1, tv2;

	//Start time
	gettimeofday(&tv1, NULL);

	for(int i=0;i<1000;i++) {
		char * temp=malloc(1);
		free(temp);
	}

	//Exit time
	gettimeofday(&tv2, NULL);

	//Return the time elapsed
	return (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
}


/* This function chooses randomly between mallocing or freeing a pointer to 1 byte. 
1000 mallocs are perfomed and eventually all pointers are freed.
*/
double workloadC() {
	struct timeval  tv1, tv2;

	//Start time
	gettimeofday(&tv1, NULL);

	//Array to hold the malloced pointers
	char * pointers[1000];

	//Number of malloc operations performed so far
	int mallocOperationCount=0;

	//Number of free operations performed so far	
	int freeOperationCount=0;

	//Number of non-freed pointers
	int numPointers=0;

	//Conitue loop until 1000 mallocs of 1 byte performed
	while(mallocOperationCount<1000) {

		//Generate randomn number of either 0 or1
		int randomNumber=random()%2;

		/*If the the randomn number is a 0, malloc 1 byte and store
		the pointer in an array. Increase the mallocOperations count
		and the count of active pointers
		*/
		if (randomNumber==0)
		{
			char * temp=malloc(1);
			mallocOperationCount++;
			pointers[numPointers]=temp;
			numPointers++;
		} 

		/* Esle if the random number is 1 and there is at least 1 active
		pointer, free the most recently allocated pointer and decrease 
		the count of active pointers. Increase the free operations count
		*/
		else if (randomNumber==1&&numPointers!=0) {
			free(pointers[numPointers-1]);
			numPointers--;
			freeOperationCount++;
		}
	}
	
	//Free all remaining active pointers
	while(freeOperationCount<1000) {
		free(pointers[numPointers-1]);
		numPointers--;
		freeOperationCount++;
	}
	

	//Exit time
	gettimeofday(&tv2, NULL);

	//Return the time elapsed
	return (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
}

/*This function randomly chooses between mallocing and freeing a pointer to 1 to 64 bytes.
1000 mallocs are perfomed. The total amount of memory allocated is kept track of so the 
total memory capacity is not exceeded. Eventually all pointers are freed.
*/
double workloadD() {
	struct timeval  tv1, tv2;

	//Start time
	gettimeofday(&tv1, NULL);

	//Array to hold the malloced pointers
	char * pointers[1000];

	//Number of malloc operations performed so far
	int mallocOperationCount=0;

	//Number of free operations performed so far	
	int freeOperationCount=0;

	//Number of non-freed pointers
	int numPointers=0;

	int totalMemoryAllocated=sizeof(Node);


	//Conitue loop until 1000 mallocs of 1 byte performed
	while(mallocOperationCount<1000) {

		//Generate randomn number of either 0 or1
		int randomNumber=random()%2;

		/*If the the randomn number is a 0, generate a random allocation
		size. If this size is too large for the amount of memory allocated
		already, continue. Malloc the allocation size and increase the
		total memory allocated int to reflect that. Store the pointer in 
		an array. Increase the mallocOperations count and the count of active 
		pointers.
		*/
		if (randomNumber==0) {
			int randomAllocationSize=random()%64+1;
			if(randomAllocationSize>(5000-totalMemoryAllocated)) {
				continue;
			}
			char * temp=malloc(randomAllocationSize);
			totalMemoryAllocated+=sizeof(Node)+randomAllocationSize;
			mallocOperationCount++;
			pointers[numPointers]=temp;
			numPointers++;
		} 

		/* Else if the random number is 1 and there is at least 1 active
		pointer, free the most recently allocated pointer and decrease 
		the count of active pointers. Increase the free operations count.
		Also decrease the total memory allocated int by the size of the 
		pointer that was freed plus the size of a metadata Node.
		*/
		else if (randomNumber==1&&numPointers>0) {
			Node * temp=(Node *)pointers[numPointers-1]-1;
			short tempSize=temp->size;
			totalMemoryAllocated-=tempSize+sizeof(Node);
			free(pointers[numPointers-1]);
			numPointers--;
			freeOperationCount++;
		}
	}
	
	//Free all remaining active pointers
	while(freeOperationCount<1000) {
		free(pointers[numPointers-1]);
		numPointers--;
		freeOperationCount++;
	}

	//Exit time
	gettimeofday(&tv2, NULL);

	//Return the time elapsed
	return (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);

}

/* Workload E consists of calling malloc 1000 times for 1 byte and storing these pointers in an array
Then, pointers are randomly selected from the array to be freed. 
*/
double workloadE() {
	struct timeval  tv1, tv2;

	//Start time
	gettimeofday(&tv1, NULL);

	//Array to hold the malloced pointers
	char * pointers[1000];

	//Make 1000 malloc calls of 1 byte
	for(int i=0;i<1000;i++) {
		pointers[i]=malloc(1);
	}

	int freeOperationCount=0;

	while(freeOperationCount<1000) {
		//Generate random number between 0 and 999
		int randomNumber=rand()%1000;

		char * ptr=pointers[randomNumber];

		//Null pointers shouldn't be freed so continue the loop
		if (ptr==NULL)
		{
			continue;
		}

		free(ptr);
		pointers[randomNumber]=NULL;
		freeOperationCount++;
	}

	//Exit time
	gettimeofday(&tv2, NULL);

	//Return the time elapsed
	return (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
}

/* Work load F chooses randomly between trying to malloc and free 1 byte char *s in a 
random spot in an array of size 1000. If the program decides to malloc but the position
it picks to store the pointer is full, the program does not malloc and the loop continues.
Likewise, the program will not try to free a pointer from a array location if it is NULL.
After a char * is malloced, the value at the pointer is set to be the ASCII value of the index
where the pointer is to be stored % 26. If that index is chosen to be freed later on, the
program checks to make sure the correct ASCII value is there. The actual character that the 
ASCII value represents does not matter, only the ASCII value. 1000 total mallocs are performed
and then all the pointers are eventually freed.
*/
double workloadF() {
	struct timeval  tv1, tv2;

	//Start time
	gettimeofday(&tv1, NULL);

	//Array to hold the malloced pointers
	char * pointers[1000];

	//Number of malloc operations performed so far
	int mallocOperationCount=0;

	//Number of free operations performed so far	
	int freeOperationCount=0;

	//Number of non-freed pointers
	int numPointers=0;

	for(int i=0;i<1000;i++) {
		pointers[i]=NULL;
	}

	while(mallocOperationCount<1000) {
		int randomNumber1=rand()%2;

		if(randomNumber1==0) {
			int randomNumberIndex=rand()%1000;
			if(pointers[randomNumberIndex]!=NULL) {
				continue;
			}

			char * ptr=malloc(sizeof(char));
			*ptr=randomNumberIndex%26;
			pointers[randomNumberIndex]=ptr;
			mallocOperationCount++;
			numPointers++;
		}

		else if(randomNumber1==1&&numPointers>0) {
			int randomNumberIndex=rand()%1000;
			if(pointers[randomNumberIndex]==NULL) {
				continue;
			}

			char * ptr=pointers[randomNumberIndex];
			if(*ptr!=randomNumberIndex%26) {
				printf("Error: Malloced memory not preserved\n");
			}
			free(ptr);
			pointers[randomNumberIndex]=NULL;
			freeOperationCount++;
			numPointers--;
		}

	}

	while(numPointers>0) {
		int randomNumberIndex=rand()%1000;
		if(pointers[randomNumberIndex]==NULL) {
			continue;
		}

		char * ptr=pointers[randomNumberIndex];
		if(*ptr!=randomNumberIndex%26) {
			printf("Error: Malloced memory not preserved\n");
		}
		free(ptr);
		pointers[randomNumberIndex]=NULL;
		freeOperationCount++;
		numPointers--;
	}

	//Exit time
	gettimeofday(&tv2, NULL);

	//Return the time elapsed
	return (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
}

/* This function executes 100 iterations of the given work load type.
It takes in a char to indicate the workoad type and also a pointer
to the workload function for the specified type.
*/
void executeWorkLoad(char name, double (*functionPtr)(void)) {
	double workloadTime[100];

	//Count of total time for all iterations
	double workloadTimeCount=0;

	printf("Beginning 100 iterations of workload %c.\n", name);

	for(int i=0;i<100;i++) {

		//Execution time of each iteration
		double executionTime=(*functionPtr)();

		workloadTime[i]=executionTime;
		workloadTimeCount+=executionTime;
	}
	double workloadAverageTime=workloadTimeCount/100;

	printf("Workload %c iterations complete.\n", name);
	printf("The total elapsed time for 100 iterations of workload %c is: %lf seconds.\n", name, workloadTimeCount);
	printf("The average elapsed time of workload %c is: %lf seconds.\n\n", name, workloadAverageTime);
}

void main(int argc, char const *argv[]) {

	//Creation of pointers to the 6 workloads
	double (*workloadAPtr)(void)=workloadA;
	double (*workloadBPtr)(void)=workloadB;
	double (*workloadCPtr)(void)=workloadC;
	double (*workloadDPtr)(void)=workloadD;
	double (*workloadEPtr)(void)=workloadE;
	double (*workloadFPtr)(void)=workloadF;

	//Execute each workload 100 times
	executeWorkLoad('A',workloadAPtr);
	executeWorkLoad('B',workloadBPtr);
	executeWorkLoad('C',workloadCPtr);
	executeWorkLoad('D',workloadDPtr);
	executeWorkLoad('E',workloadEPtr);
	executeWorkLoad('F',workloadFPtr);


	printf("Memgrind Complete.\n");
}