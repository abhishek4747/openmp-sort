#include <iostream>
#include <omp.h>
#include "mergesort.h"

using namespace std;

void  merge  (dataType *datal , int sizel , dataType *datar , int sizer , dataType *data2){
    int i1 = 0, i2 = 0, i3 = 0;
       
    while (i1 < sizel && i2 < sizer) {
        if (datal[i1].key < datar[i2].key) {
            data2[i3++] = datal[i1++];
        } 
        else {
            data2[i3++] = datar[i2++];
        }
    }
    while (i1 < sizel) {   
        data2[i3++] = datal[i1++];
    }
    while (i2 < sizer) { 
        data2[i3++] = datar[i2++];
    }

    int size = sizel + sizer;
    //#pragma omp parallel for private(size)
    for (int i = 0; i < size; ++i)
    {
    	datal[i] = data2[i];
    }
}


void mergesort(dataType *data, int size, dataType *data2){
	if (size<2){
		return;
	}else{
		//#pragma omp parallel num_threads(2)
		//{	
		//	if (omp_get_thread_num()==0)
				mergesort(data,size/2,data2);
		//	else
				mergesort(data+size/2,(size+1)/2,data2+size/2);
		//}
		merge(data,size/2,data+size/2, (size+1)/2,data2);
	}
}