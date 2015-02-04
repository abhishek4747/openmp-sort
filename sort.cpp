#include <iostream>						// For IO
#include <omp.h>						// For openmp
#include <stdlib.h>						// For exit(0)
#include "sort.h"
#include "quicksort.h"					// For quicksort
#include "mergesort.h"					// For mergesort
#include "radixsort.h"					// For radixsort
#include "othersort.h"					// For othersort

using namespace std;

void pSort(dataType *data, int ndata, SortType sorter){
	dataType *data2;
	switch (sorter){
		case QUICK:
			quicksort(data, 0, ndata);
			break;
		case MERGE:
			data2=(dataType*)malloc(ndata*sizeof(dataType));
			if (data2==0){
				cout<<"OUT OF MEMORY\n";
				exit(0);
			}
			mergesort(data,ndata,data2);
			free(data2);
			break;
		case RADIX:
			data2=(dataType*)malloc(ndata*sizeof(dataType));
			if (data2==0){
				cout<<"OUT OF MEMORY\n";
				exit(0);
			}
			radixsort(data,ndata,data2);
			free(data2);
			break;
		default:
			othersort();
	}
}

