#include <iostream>						// For IO
#include <time.h>						// For timing
#include <stdlib.h>						// For exit(0)
#include <omp.h>
#include "sort.h"
#include <bitset>
#include <set>

using namespace std;

// GLOBALS
SortType sorttype = QUICK;
int NUM_OF_ELEMENTS =  1<<20;
int MIN_NUM = 1<<20;
int isParallel = 1;

//#define BINARY 1

void arrayPrint(dataType *data, int n = NUM_OF_ELEMENTS){
	for (int i = 0; i < n; ++i){
		//cout<<(long long)data[i].key<<"\t";
		cout<<(bitset<64>) ((long long) data[i].key)<<"\n";
		
	}
	cout<<"\n\n";
}

int isSorted(dataType* data, int n = NUM_OF_ELEMENTS){
	long long *first = data[0].key;
	for (int i = 1; i < n; ++i){
		if ((long long) data[i-1].key > (long long) data[i].key){
			return 0;
		}
	}
	return 1;
}

/* let the main begin */
int main(int argc, char *argv[]){
	
	/* my timing toolset */
	clock_t begin, end;
	double time_spent;
	cout<<"\n\n\n\n";
	/* set global args as per passed args */
	if (argc>1){
		NUM_OF_ELEMENTS = 1 << atoi(argv[1]);
	}
	if (argc>2){
		isParallel = atoi(argv[2]);
		if (!isParallel)
			cout<<"**** NOT PARALLEL ****\n";
	}
	if (argc>3){
		switch (argv[3][0]){
			case 'q':
				sorttype = QUICK;
				cout<<"\t\tQUICKSORT\n";
				break;
			case 'm':
				sorttype = MERGE;
				cout<<"\t\tMERGESORT\n";
				break;
			case 'r':
				sorttype = RADIX;
				cout<<"\t\tRADIXSORT\n";
				break;
			default:
				sorttype = BEST;
				cout<<"\t\tBESTSORT\n";
		}
	}

	/* declare and allocate array of the hour */
	dataType *data;
	data=(dataType*)malloc(NUM_OF_ELEMENTS*sizeof(dataType));
	if (data==0){
		cout<<"OUT OF MEMORY.\n";
		exit(0);
	}else{
		cout<<"Sorting "<<NUM_OF_ELEMENTS<<" integers.\n";
	}

	
	srand(time(NULL));
	/* fill array with random numbers */
	/* -------------------------- */
	srand(rand());srand(rand());
	begin = clock();
	unsigned seed;
	#pragma omp parallel private(seed) if (isParallel)
	{	
		seed = rand()*omp_get_thread_num();
		#pragma omp parallel for 
		for (int i = 0; i < NUM_OF_ELEMENTS; ++i){
			int r = rand_r(&seed);
			//data[i].key = (long long *)(((((long long)(rand_r(&seed))<<31)|(long long)r)<<2)|(long long)(r>>29));
			//data[i].key = (long long *)(((long long)(rand_r(&seed))<<31)|r);
			data[i].key = (long long *) r;
		}
	}
	set <long long> s1;
	if (NUM_OF_ELEMENTS < MIN_NUM){
		for (int i = 0; i < NUM_OF_ELEMENTS; ++i){
			s1.insert((long long)data[i].key);
		}
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout<<"Time: "<<time_spent<<" seconds to fill the array.\n";
	if (NUM_OF_ELEMENTS< 1<<4)
		arrayPrint(data);
	
	/* -------------------------- */

	
	/* sort array */
	/* -------------------------- */
	begin = clock();	
		pSort(data,NUM_OF_ELEMENTS,sorttype);
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout<<"Time: "<<time_spent<<" seconds to sort the array.\n";
	/* -------------------------- */

	/* check array */
	/* -------------------------- */
	begin = clock();	
	set <long long> s2;	
	if (NUM_OF_ELEMENTS < MIN_NUM){
		for (int i = 0; i < NUM_OF_ELEMENTS; ++i){
			s2.insert((long long)data[i].key);
		}
	}
	if (!isSorted(data)){
		cout<<"ARRAY NOT SORTED!!\n";
	}
	if (s1.size()!=s2.size()){
		cout<<"ARRAY CORRUPTED!!\n";
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout<<"Time: "<<time_spent<<" seconds to check if array is sorted.\n";
	if (NUM_OF_ELEMENTS< 1<<4)
		arrayPrint(data);
	
	/* -------------------------- */
	
	free(data);
	return 0;
}

/* silence is gold */
