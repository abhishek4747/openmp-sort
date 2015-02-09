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

    //int size = sizel + sizer;
    //#pragma omp parallel for
    //for (int i = 0; i < size; ++i)
    //{
		//cout<<(long long)datal[i].key<<", ";
    //	datal[i] = data2[i];
		//if (i==sizel)
		//	cout<<" --- ";
    //}
	//cout<<endl;

	//for (int i =0; i<size; ++i){
		//cout<<(long long)datal[i].key<<", ";	
	//}
	//cout<<endl<<endl;
}


void mergesort(dataType *data, int size, dataType *data2){
	if (size>1){
		//#pragma omp parallel if (size>8 && false)
		{	
			//#pragma omp single  
			{
				//#pragma omp task 
				{
					mergesort(data,size/2,data2);
				}
				//#pragma omp task 
				{
					mergesort(data+size/2,(size+1)/2,data2+size/2);
				}
			}
		}
		merge(data2,size/2,data2+size/2, (size+1)/2,data);
	}
}
