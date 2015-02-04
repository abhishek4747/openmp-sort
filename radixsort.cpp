#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bitset>
#include "radixsort.h"

using namespace std;

#define R 1
#define RADIX (1<<R)
#define BUCKETS RADIX

int shift(int digit){
	return 64 - R*(1 + digit);
}

long long applymask(long long key, int digit){
	int s  = shift(digit);
	return (key & (((long long)(RADIX-1))<<s))>>s;
}

void radixsort(dataType *data, int ndata, dataType *data2, int digit){
	if (digit>(64/R)-1){
		return;
	}
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

	for (int i = 0; i < ndata; ++i){
		int a = applymask((long long)data[i].key, digit);
		bsize[a] += 1;		
	}
	bstart[0] = bstart2[0] = 0;
	for (int i = 1; i < BUCKETS; ++i){
		bstart[i] = bsize[i-1] + bstart[i-1];
		bstart2[i] = bstart[i];
		// cout<<bstart[i]<<" -- "<<bstart2[i]<<endl;
	}
	// memcpy(bstart2, bstart, BUCKETS*sizeof(int));

	// cout<<"Digit: "<<digit<<" shift: "<<shift(digit)<<" ";
	// for (int i = 0; i < BUCKETS; ++i){
	// 	cout<<bsize[i]<<","<<bstart2[i]<<"\t";
	// }
	// cout<<endl;
	
	for (int i = 0; i < ndata;++i){
		int a = applymask((long long)data[i].key,digit);
		// cout<<" a:"<<a;
		// cout<<" bstart[a]:"<<bstart[a];
		// cout<<" data["<< i <<"]:"<< ((bitset<16>)(long long)data[i].key) << endl;
		data2[bstart[a]] = data[i];
		bstart[a]++;
	}
	for (int i = 0; i < ndata; ++i){
		data[i] = data2[i];
	}
	for (int i = 0; i < BUCKETS; ++i){
		if (bsize[i]>0){
			// cout<<"digit:"<<digit<<" Bucket:"<<i<<" start:"<<bstart2[i]<<" size:"<<bsize[i]<<endl;
			radixsort(data+bstart2[i],bsize[i],data2,digit+1);
		}
	}
}