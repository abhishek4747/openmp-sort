#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "psum.h"

using namespace std;

void psum(int *data, int size, int *data2){
	if (size<2)
		return;
	// Pairwise sum
	int s = size>>1;
	#pragma omp parallel for
	for (int i=0; i< s; i++){
		data2[i] = data[2*i] + data[2*i+1];
	}
	// Recurse
	psum(data2, s, data2+s);

	// Finalize
	#pragma omp parallel for
	for (int i=0; i<s; i++){
		data[2*i] = data2[i] -data[2*i+1];
		data[2*i + 1] = data2[i];
	}

	if (size&1){
		data[size-1] = data[size-1] + data[size -2];
	}
}

/*
void swap(dataType *data, int left, int right){
	dataType temp = data[left];
	data[left] = data[right];
	data[right] = temp;
}

	
void partition(dataType *data, int size){
	int p = omp_get_num_procs();
	int *lessp;
	int *greaterp;
	lessp = (int*) malloc(p*sizeof(int));
	greaterp = (int*)malloc(p*sizeof(int));
	if( lessp == 0 || greaterp ==0){
		cout<<"OUT OF MEMORY."<<endl;
		exit(0);
	}
	int *lessp2;
	int *greaterp2;
	lessp2 = (int*) malloc(p*sizeof(int));
	greaterp2 = (int*)malloc(p*sizeof(int));
	if( lessp2 == 0 || greaterp2 ==0){
		cout<<"OUT OF MEMORY."<<endl;
		exit(0);
	}
	#pragma omp parallel
	{
		int i = ome_get_thread_num();
		int start = (size/p)*i;
		int end = min(ceil(size/p)*(i+1),size);
		int pi = end-1;
		int store = start;
		swap(data, start, pi);
		for (int i = start; i<end-1; i++){
			if ((long long)data[i].key<=(long long)data[pi].key){
				swap(data, i, store);
				store++;
			}
		}	
		swap(data, pi, store);
		lessp[i]=store;
		greater[i] = end-start - store;
	}
	psum(lessp,p,lessp2);
	psum(greaterp,p,greaterp2);
}
*/
int main(){
	int size = 20;
	int  *data;
	data = (int *) malloc(size*sizeof(int));
	int *data2;
	data2 = (int *) malloc(size*sizeof(int));
	data[0] = 7;
	data[1] = 13;
	data[2] = 18;
	data[3] = 2;
	data[4] = 17;
	data[5] = 1;
	data[6] = 14;
	data[7] = 20;
	data[8] = 6;
	data[9] = 7;
	data[10] = 10;
	data[12] = 15;
	data[13] = 9;
	data[14] = 3;
	data[15] = 16;
	data[16] = 19;
	data[17] = 4;
	data[18] = 11;
	data[19] = 12;
	/*
	data[0].key = (long long*) 7;
	data[1].key = (long long*) 13;
	data[2].key = (long long*) 18;
	data[3].key = (long long*) 2;
	data[4].key = (long long*) 17;
	data[5].key = (long long*) 1;
	data[6].key = (long long*) 14;
	data[7].key = (long long*) 20;
	data[8].key = (long long*) 6;
	data[9].key = (long long*) 10;
	data[10].key = (long long*) 15;
	data[11].key = (long long*) 9;
	data[12].key = (long long*) 3;
	data[13].key = (long long*) 16;
	data[14].key = (long long*) 19;
	data[15].key = (long long*) 4;
	data[16].key = (long long*) 11;
	data[17].key = (long long*) 12;
	data[18].key = (long long*) 5;
	data[19].key = (long long*) 8;
	*/
	for (int i = 0; i<size; i++){
		cout<<(long long)data[i]<<", ";
	}
	cout<<endl;

	psum(data, size, data2);

	for (int i = 0; i<size; i++){
		cout<<(long long)data[i]<<", ";
	}
	cout<<endl;
	return 0;
}
