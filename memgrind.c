#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>

#include "mymalloc.h"


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


//Fix
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
		Also drecrease the total memory allocated int by the size of the 
		pointer that was freed plus the size of a metadata Node.
		*/
		else if (randomNumber==1&&numPointers!=0) {
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


void main(int argc, char const *argv[]) {
	double workloadATime[100];
	double workloadATimeCount=0;
	printf("Beginning 100 iterations of workload A.\n");
	for(int i=0;i<100;i++) {
		double executionTime=workloadA();
		workloadATime[i]=executionTime;
		workloadATimeCount+=executionTime;
	}
	double workloadAAverageTime=workloadATimeCount/100;
	printf("Workload A iterations complete.\n");
	printf("The total elapsed time for 100 iterations of workload A is: %lf seconds.\n",workloadATimeCount);
	printf("The average elapsed time of workload A is: %lf seconds.\n\n",workloadAAverageTime);

	double workloadBTime[100];
	double workloadBTimeCount=0;
	printf("Beginning 100 iterations of workload B.\n");
	for(int i=0;i<100;i++) {
		double executionTime=workloadB();
		workloadBTime[i]=executionTime;
		workloadBTimeCount+=executionTime;
	}
	double workloadBAverageTime=workloadBTimeCount/100;
	printf("Workload B iterations complete.\n");
	printf("The total elapsed time for 100 iterations of workload B is: %lf seconds.\n", workloadBTimeCount);
	printf("The average elapsed time of workload B is: %lf seconds.\n\n",workloadBAverageTime);


	double workloadCTime[100];
	double workloadCTimeCount=0;
	printf("Beginning 100 iterations of workload C.\n");
	for(int i=0;i<100;i++) {
		double executionTime=workloadC();
		workloadCTime[i]=executionTime;
		workloadCTimeCount+=executionTime;
	}
	double workloadCAverageTime=workloadCTimeCount/100;
	printf("Workload C iterations complete.\n");
	printf("The total elapsed time for 100 iterations of workload C is: %lf seconds.\n", workloadCTimeCount);
	printf("The average elapsed time of workload C is: %lf seconds.\n\n",workloadCAverageTime);


	double workloadDTime[100];
	double workloadDTimeCount=0;
	printf("Beginning 100 iterations of workload D.\n");
	for(int i=0;i<100;i++) {
		double executionTime=workloadD();
		workloadDTime[i]=executionTime;
		workloadDTimeCount+=executionTime;
	}
	double workloadDAverageTime=workloadDTimeCount/100;
	printf("Workload D iterations complete.\n");
	printf("The total elapsed time for 100 iterations of workload D is: %lf seconds.\n", workloadDTimeCount);
	printf("The average elapsed time of workload D is: %lf seconds.\n\n",workloadDAverageTime);
	
}