#include "pquick.h"
#include "helper.h"
#include "quicksort.h"

using namespace std;

int procs, osize;

// must be called from one thread only
void psum(int *data, int size, int *data2){
	if (size<2)
		return;

	// #1: Pairwise sum
	int s = size>>1;
	#pragma omp parallel for
	for (int i=0; i< s; i++){
		data2[i] = data[2*i] + data[2*i+1];
	}
	// #2: Recurse
	psum(data2, s, data2+s);

	// #3: Finalize
	#pragma omp parallel for
	for (int i=0; i<s; i++){
		data[2*i] = data2[i] -data[2*i+1];
		data[2*i + 1] = data2[i];
	}

	if (size&1){
		data[size-1] = data[size-1] + data[size -2];
	}
}



int splitp ( dataType *a, int upper , long long pivot){
	int  p, q;
	p =  0 ;
	q = upper - 1 ;

	while ( q >= p ){
		while (q>=p && getkey(a,p)<= pivot )
			p++ ;

		while (q>=p && getkey(a,q)> pivot )
			q-- ;

		if ( q > p )
			swap(a,p,q);
	}
	return q ;
}

	
int partition(dataType *data, int size, dataType *data2){
	if (size<2)
		return 0;
	else if (/*size <osize/procs ||*/ size < 1<<24){
		return splitq(data, size);
	} else{
		int p = procs;
		int *lessp;
		lessp = (int*) malloc(p*sizeof(int));
		if( lessp == 0 ){
			cout<<"OUT OF MEMORY."<<endl;
			exit(0);
		}
		int *lessp2;
		lessp2 = (int*) malloc(p*sizeof(int));
		if( lessp2 == 0 ){
			cout<<"OUT OF MEMORY."<<endl;
			exit(0);
		}
		int chunk = ceil(((double)size)/p);
		p = ceil(((double)size)/chunk);

		#pragma omp parallel num_threads(p) 
		{
			int i = omp_get_thread_num();
			int start = chunk*i;
			int end = min((int)chunk*(i+1),size);

			lessp[i] = splitp(data+start, end-start, getkey(data,0))+1;	
		}
		
		psum(lessp,p,lessp2);
		free(lessp2);

		#pragma omp parallel num_threads(p)
		{
			int i = omp_get_thread_num();
			int start = chunk*i;
			int end = min((int)chunk*(i+1),size);
			int lp = (i>0? lessp[i-1]:0);
			int dp = lessp[i];
			
			for (int j = 0; j<dp-lp; j++){
				data2[lp+j] = data[start+j];
			}
			int gp = (i>0?( start - lp):0);
			int gd = end - dp;
			for (int j =0; j <gd-gp; j++){
				data2[lessp[p-1]+gp+j] = data[start+dp-lp+j];
			}
		}
		
		swap(data2,0,lessp[p-1]-1);	
		
		#pragma omp parallel for num_threads(p)
		for (int i=0; i<size; i++){
			data[i] = data2[i];
		}
		return (lessp[p-1]>0?lessp[p-1]-1:0);
	}
}

void pqsort(dataType *data, int start, int end, dataType *data2){
	if (start+1>=end){
		return;
	}else if (start+2==end){
		if ( getkey(data,start)>  getkey(data,start+1)){
			swap(data, start, start+1);
		}
	}else if (start+1<end){
		int size = end - start;
		int store = start + partition(data+start, size, data2+start);
		//int store = start + splitq(data+start, size);
		#pragma omp task if (size>1<<20)
		{
			pqsort(data, start, store, data2);
		}
		#pragma omp task if (size>1<<20)
		{
			pqsort(data, store+1, end, data2);
		}
	}
}

void pquicksort(dataType *data, int size){
	osize = size;
	procs = omp_get_num_procs();
	dataType *data2;
	data2 = (dataType*) malloc(size*sizeof(dataType));
	if (data2==0){
		cout<<"OUT OF MEMORY"<<endl;
		exit(0);
	}
	omp_set_nested(1);
	#pragma omp parallel
	{
		#pragma omp single nowait
		{
			pqsort(data, 0, size, data2);
		}
	}
	free(data2);
}

