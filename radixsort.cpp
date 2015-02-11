#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bitset>
#include "radixsort.h"
#include "helper.h"

using namespace std;

#define R 4
#define RADIX (1<<R)
#define BUCKETS RADIX

int shift(int digit){
	return 64 - R*(1 + digit);
}

long long applymask(long long key, int digit){
	int s  = shift(digit);
	return (key & (((long long)(RADIX-1))<<s))>>s;
}


void make_buckets(dataType *data, int size, dataType *data2,int digit, int *bsize, int *bstart, int *bstart2){


	for (int i = 0; i < size; ++i){
		int a = applymask((long long)data[i].key, digit);
		bsize[a] += 1;		
	}
	bstart[0] = bstart2[0] = 0;
	for (int i = 1; i < BUCKETS; ++i){
		bstart[i] = bsize[i-1] + bstart[i-1];
		bstart2[i] = bstart[i];
		// cout<<bstart[i]<<" -- "<<bstart2[i]<<endl;
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
	bsize = (int*) malloc(BUCKETS*sizeof(int));
	bstart = (int*) malloc(BUCKETS*sizeof(int));
	bstart2 = (int*) malloc(BUCKETS*sizeof(int));
	if (bsize==0 || bstart==0 || bstart2==0){
		cout<<"OUT OF MEMORY.\n";
		exit(0);
	}
	memset(bsize, 0, BUCKETS*sizeof(int));

	make_buckets(data, size, data2, digit, bsize, bstart, bstart2);
	// memcpy(bstart2, bstart, BUCKETS*sizeof(int));

	// cout<<"Digit: "<<digit<<" shift: "<<shift(digit)<<" ";
	// for (int i = 0; i < BUCKETS; ++i){
	// 	cout<<bsize[i]<<","<<bstart2[i]<<"\t";
	// }
	// cout<<endl;
	
	for (int i = 0; i < size;i++){
		data[i] = data2[i];
	}
	for (int i = 0; i < BUCKETS; ++i){
		if (bsize[i]>0){
			// cout<<"digit:"<<digit<<" Bucket:"<<i<<" start:"<<bstart2[i]<<" size:"<<bsize[i]<<endl;
			//cout<<"Bucket: "<<i<<" Digit:"<<digit<<endl;
			#pragma omp task if (bsize[i]> 1<<20)
			{
				rsort(data+bstart2[i],bsize[i],data2,digit+1);
			}
		}
	}
#pragma omp taskwait
	free(bsize);
	free(bstart);
	free(bstart2);
}


void radixsort(dataType *data, int size, dataType *data2, int digit){
	#pragma omp parallel
	{
		#pragma omp single
		{
			rsort(data, size, data2, digit);
		}
	}
}
