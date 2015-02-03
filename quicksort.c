#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int NUM_OF_ELEMENTS =  1<<16;

void arrayPrint(int elem[]){
	int i;
	for (i = 0; i < NUM_OF_ELEMENTS; ++i){
		printf("%d,\t",elem[i]);
	}
	printf("\n");
}

int isSorted(int elem[], int n){
	int i,first = elem[0];

	for (i = 1; i < n; ++i){
		if (first > elem[i]){
			return 0;
		}
	}
	return 1;
}

void quicksort(int elem[], int start, int end){
	if (start+1>=end){
		return;
	}else{
		int middle = start;
		int iter = start+1;
		int sorted = 1;
		while(iter!=end){
			if (elem[middle]>elem[iter]){
				int temp = elem[iter];
				elem[iter] = elem[middle];
				elem[middle] = temp;
				middle++;
				sorted = 0;
			}
			iter++;
		}
		if (!sorted && start+1<=middle){
			quicksort(elem, start, middle);
		}
		if (!sorted && middle+1<=end){
			quicksort(elem, middle, end);
		}
	}
}

/* let the main begin */
int main(int argc, char *argv[]){
	
	/* my timing toolset */
	clock_t begin, end;
	double time_spent;
	int i;

	/* set NUM_OF_ELEMENTS as per arguments */
	if (argc>1){
		NUM_OF_ELEMENTS = 1 << atoi(argv[1]);
	}

	/* declare and allocate array of the hour */
	int *elem;
	elem=(int*)malloc(NUM_OF_ELEMENTS*sizeof(int));
	if (elem==0){
		printf("OUT OF MEMORY.\n");
		exit(0);
	}else{
		printf("Sorting %d integers.\n",NUM_OF_ELEMENTS );
	}

	/* put a seed for random */
	srand(time(NULL));srand(rand());srand(rand());

	/* fill array with random numbers */
	/* -------------------------- */
	begin = clock();	
		for (i = 0; i < NUM_OF_ELEMENTS; ++i){
			elem[i] = rand();
		}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Time: %lf seconds to fill the array.\n",time_spent );
	/* -------------------------- */

	
	/* sort array */
	/* -------------------------- */
	begin = clock();	
		quicksort(elem,0,NUM_OF_ELEMENTS);
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Time: %lf seconds to sort the array.\n",time_spent );
	/* -------------------------- */

	/* check array */
	/* -------------------------- */
	begin = clock();	
		if (!isSorted(elem, NUM_OF_ELEMENTS)){
			printf("ARRAY NOT SORTED!!\n");
		}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Time: %lf seconds to check if array is sorted.\n",time_spent);
	/* -------------------------- */
	
	return 0;
}

/* silence is gold */
