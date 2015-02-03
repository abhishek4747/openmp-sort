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
	
	/* declare array of the hour */
	int *elem;
	
	/* my timing toolset */
	clock_t begin, end;
	double time_spent;

	if (argc>1){
		NUM_OF_ELEMENTS = 1 << atoi(argv[1]);
	}

	/* allocate space to array */
	elem=(int*)malloc(NUM_OF_ELEMENTS*sizeof(int));
	if (elem==0){
		printf("OUT OF MEMORY.\n");
		exit(0);
	}

	/* put a seed for random */
	srand(time(NULL));srand(rand());srand(rand());

	/* fill array with random numbers */
	int i;
	for (i = 0; i < NUM_OF_ELEMENTS; ++i){
		elem[i] = rand();
	}

	
	/* -------------------------- */
	begin = clock();	
		quicksort(elem,0,NUM_OF_ELEMENTS);
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Time: %lf\n",time_spent );
	/* -------------------------- */
	
	return 0;
}

/* silence is gold */
