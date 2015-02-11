#include <iostream>
#include <omp.h>
#include "quicksort.h"
#include "helper.h"


using namespace std;

inline void swap(dataType *data, int left, int right){
	dataType temp = data[left];
	data[left] = data[right];
	data[right] = temp;
}

int splitq ( dataType *a, int upper ){
	int  p, q;
	p =  1 ;
	q = upper - 1 ;

	while ( q >= p ){
		while ((long long) a[p].key < (long long) a[0].key )
			p++ ;

		while ((long long) a[q].key > (long long) a[0].key )
			q-- ;

		if ( q > p )
			swap(a,p,q);
	}
	swap(a,0,q);
	return q ;
}

void qsort(dataType *data, int start, int end){
	if (start+1>=end){
		return;
	} else if (start+2==end){
		if ((long long)data[start].key> (long long)data[start+1].key){
			swap(data, start, start+1);
		}
	}
	else if (start+1<end){
		int pi = end-1;
		int store = start;
		swap(data, (start+end)>>1, pi);
		for (int i = start; i<end-1; i++){
			if ((long long)data[i].key<=(long long)data[pi].key){
				swap(data, i, store);
				store++;
			}
		}	
		swap(data, pi, store);
		int size = end - start;
//		int store = start + splitq(data+start, size);
		#pragma omp task if (size > 1 <<20)
		{
			qsort(data, start, store);
		}
		#pragma omp task if (size > 1 <<20)
		{
			qsort(data, store+1, end);
		}
	}
}

void quicksort(dataType *data, int size){
	#pragma omp parallel
	{
		#pragma omp single nowait
		{
			qsort(data, 0, size);
		}
	}
}
