#include "quicksort.h"
#include "helper.h"


using namespace std;

int splitq ( dataType *a, int upper ){
	int  p, q;
	p =  0 ;
	q = upper - 1 ;

	while ( q >= p ){
		while (q>=p &&  getkey(a,p) <=  getkey(a,0) )
			p++ ;

		while (q>=p &&  getkey(a,q) >  getkey(a,0) )
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
		if ( getkey(data,start)>  getkey(data,start+1)){
			swap(data, start, start+1);
		}
	}
	else if (start+1<end){
		int size = end - start;
		int store = start + splitq(data+start, size);
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
