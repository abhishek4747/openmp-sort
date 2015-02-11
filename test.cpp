#include <iostream>						// For IO
#include <stdlib.h>						// For exit(0)
#include <omp.h>
#include <bitset>
#include <set>
#include "sort.h"
#include "helper.h"

using namespace std;

// GLOBALS
#define DEBUG 1
SortType sorttype = QUICK;
int NUM_OF_ELEMENTS =  1<<20;
int MIN_NUM = 1<<20;
int isParallel = 1;

//#define BINARY 1

void arrayPrint(dataType *data, int n = NUM_OF_ELEMENTS){
	for (int i = 0; i < n; ++i){
		if (sorttype!=RADIX)
			cout<<getkey(data,i)<<"\t";
		else
			cout<</*(bitset<64>)*/ getkey(data, i)<<"\n";
		
	}
	cout<<"\n"<<endl;
}

int isSorted(dataType* data, int n = NUM_OF_ELEMENTS){
	for (int i = 1; i < n; ++i){
		if (getkey(data,i-1) > getkey(data,i)){
			return 0;
		}
	}
	return 1;
}

/* let the main begin */
int main(int argc, char *argv[]){
	
	/* my timing toolset */
	double begin, end;
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
				cout<<"\t\tQUICKSORT"<<endl;
				break;
			case 'm':
				sorttype = MERGE;
				cout<<"\t\tMERGESORT"<<endl;
				break;
			case 'r':
				sorttype = RADIX;
				cout<<"\t\tRADIXSORT"<<endl;
				break;
			default:
				sorttype = BEST;
				cout<<"\t\tBESTSORT"<<endl;
		}
	}

	/* declare and allocate array of the hour */
	dataType *data;
	data=(dataType*)malloc(NUM_OF_ELEMENTS*sizeof(dataType));
	if (data==0){
		cout<<"OUT OF MEMORY.\n";
		exit(0);
	}

	
	srand(time(NULL));
	/* fill array with random numbers */
	/* -------------------------- */
	srand(rand());srand(rand());
	begin = omp_get_wtime();
	unsigned seed;
	#pragma omp parallel private(seed) if (isParallel)
	{	
		seed = rand()*omp_get_thread_num();
		#pragma omp parallel for 
		for (int i = 0; i < NUM_OF_ELEMENTS; ++i){
			int r = rand_r(&seed);
			data[i].key = (long long *)(((((long long)(rand_r(&seed))<<31)|(long long)r)<<2)|(long long)(r>>29));
			 //data[i].key = (long long *)(((long long)(rand_r(&seed))<<31)|r);
			// data[i].key = (long long *)(long long)r;
		}
	}
	set <long long> s1;
	if (NUM_OF_ELEMENTS < MIN_NUM){
		for (int i = 0; i < NUM_OF_ELEMENTS; ++i){
			s1.insert(getkey(data,i));
		}
	}
	end = omp_get_wtime();
	time_spent = (double)(end - begin);
	cout<<"Time: "<<time_spent<<" seconds to fill the array with "<<NUM_OF_ELEMENTS<<" numbers."<<endl;
	if (NUM_OF_ELEMENTS< 1<<4)
		arrayPrint(data);
	
	/* -------------------------- */

	
	/* sort array */
	/* -------------------------- */
	begin = omp_get_wtime();	
		pSort(data,NUM_OF_ELEMENTS,sorttype);
	end = omp_get_wtime();
	time_spent = (double)(end - begin);
	cout<<"Time: "<<time_spent<<" seconds to sort the array of "<<NUM_OF_ELEMENTS<<" numbers."<<endl;
	/* -------------------------- */

	/* check array */
	/* -------------------------- */
	begin = omp_get_wtime();	
	set <long long> s2;	
	if (NUM_OF_ELEMENTS < MIN_NUM){
		for (int i = 0; i < NUM_OF_ELEMENTS; ++i){
			s2.insert(getkey(data,i));
		}
	}
	if (!isSorted(data)){
		cout<<"ARRAY NOT SORTED!!\n";
	}
	if (s1.size()!=s2.size()){
		cout<<"ARRAY CORRUPTED!!\n";
	}
	end = omp_get_wtime();
	time_spent = (double)(end - begin);
	cout<<"Time: "<<time_spent<<" seconds to check if array is sorted."<<endl;
	if (NUM_OF_ELEMENTS< 1<<4)
		arrayPrint(data);
	
	/* -------------------------- */
	
	free(data);
	return 0;
}

/* silence is gold */
