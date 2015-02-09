#include <iostream>
#include <omp.h>
#include "mergesort.h"

using namespace std;

inline void  merge  (dataType *datal , int sizel , dataType *datar , int sizer , dataType *data2){
    int i1 = 0, i2 = 0, i3 = 0;
       
    while (i1 < sizel && i2 < sizer) {
        if (datal[i1].key < datar[i2].key) {
            data2[i3++] = datal[i1++];
        } 
        else {
            data2[i3++] = datar[i2++];
        }
    }
	//#pragma omp parallel
	{
		//#pragma omp sections
		{
			//#pragma omp section
			while (i1 < sizel) {   
				data2[i3++] = datal[i1++];
			}
			//#pragma omp section
			while(sizer-1>=i2){
				--sizer;
				data2[sizel + sizer] = datar[sizer];
			}
		}
	}	
    //while (i2 < sizer) { 
    //    data2[i3++] = datar[i2++];
    //}

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
		#pragma omp parallel if (size> 1<<10) num_threads(4)
		{	
			#pragma omp sections 
			{
				#pragma omp section
				{
					mergesort(data2,size/2,data);
				}
				#pragma omp section
				{
					mergesort(data2+size/2,(size+1)/2,data+size/2);
				}
			}
		}
		merge(data2,size/2,data2+size/2, (size+1)/2,data);
	}
}
