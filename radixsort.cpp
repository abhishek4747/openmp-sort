#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bitset>
#include "radixsort.h"
#include "helper.h"

using namespace std;

#define R 3
#define RADIX (1<<R)

int shift(int digit){
	return 64 - R*(1 + digit);
}

long long applymask(long long key, int digit){
	int s  = shift(digit);
//	return (key & (((long long)(RADIX-1))<<s))>>s;
	long long ret =  ((key>>s) & ((long long)(RADIX-1)));
	if (digit==0){
		if (ret<(RADIX>>1))
			return (RADIX>>1) + ret;
		else
			return ret- (RADIX>>1);
	}
	else
		return ret;
}


void make_buckets(dataType *data, int size, dataType *data2,int digit, int *bsize, int *bstart, int *bstart2){


	for (int i = 0; i < size; ++i){
		int a = applymask(getkey(data,i), digit);
		bsize[a] += 1;
		//cout<<(bitset<64>)getkey(data,i)<<" "<<a<<" "<<RADIX-1-a<<endl;		
	}
	/*
	cout<<"Digit:"<<digit<<endl;
	for (int i= 0; i<RADIX; ++i){
		cout<<bsize[i]<<", ";
	}
	cout<<endl;
	*/
	bstart[0] = bstart2[0] = 0;
	for (int i = 1; i < RADIX; ++i){
		bstart[i] = bsize[i-1] + bstart[i-1];
		bstart2[i] = bstart[i];
		//cout<<bstart[i]<<" -- "<<bstart2[i]<<endl;
	}
	for (int i = 0; i < size;++i){
		int a = applymask((long long)data[i].key,digit);
		// cout<<" a:"<<a;
		// cout<<" bstart[a]:"<<bstart[a];
		// cout<<" data["<< i <<"]:"<< ((bitset<16>)(long long)data[i].key) << endl;
		data2[bstart[a]] = data[i];
		bstart[a]++;
	}
}

void rsort(dataType *data, int size, dataType *data2, int digit){
	if (digit>(64/R)-1)
		return;
	if (size<2)
		return;
	int *bsize;
	int *bstart;
	int *bstart2;
	//#pragma omp critical
	{
		bsize = (int*) malloc(RADIX*sizeof(int));
		bstart = (int*) malloc(RADIX*sizeof(int));
		bstart2 = (int*) malloc(RADIX*sizeof(int));
		if (bsize==0 || bstart==0 || bstart2==0){
			cout<<"OUT OF MEMORY.\n";
			exit(0);
		}
	}
	memset(bsize, 0, RADIX*sizeof(int));

	make_buckets(data, size, data2, digit, bsize, bstart, bstart2);
	for (int i = 0; i < size;i++){
		data[i] = data2[i];
	}
	//#pragma omp for
	for (int i = 0; i < RADIX; ++i){
		#pragma omp task if (bsize[i]> 1<<20)
		{
			rsort(data+bstart2[i],bsize[i],data2+bstart2[i],digit+1);
		}
	}
	#pragma omp taskwait
	free(bsize);
	free(bstart);
	free(bstart2);
}


void radixsort(dataType *data, int size){
	dataType *data2 = (dataType*)malloc(size*sizeof(dataType));
	if (data2==0){
		cout<<"OUT OF MEMORY\n";
		exit(0);
	}
	omp_set_nested(1);
	#pragma omp parallel
	{
		#pragma omp single
		{
			rsort(data, size, data2, 0);
		}
	}
}
