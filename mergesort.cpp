#include "mergesort.h"
#include "helper.h"

using namespace std;

inline void  merge  (dataType *datal , int sizel , dataType *datar , int sizer , dataType *data2){
    int i1 = 0, i2 = 0, i3 = 0;
       
    while (i1 < sizel && i2 < sizer) {
        if ((long long) datal[i1].key < (long long)datar[i2].key) {
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
			//int i1o = i3 - i1;
			//#pragma omp parallel for 
			//for (int i=i1;i < sizel;i++) {   
			//	data2[i + i1o] = datal[i];
			//}
			while(i1<sizel)
				data2[i3++] = datal[i1++];
			while(i2<sizel)
				data2[i3++] = datar[i2++];
			//#pragma omp section
			//for(;sizer-1>=i2;){
			//	--sizer;
			//	data2[sizel + sizer] = datar[sizer];
			//}
			//int i2o = i3 - i2;
			//#pragma omp parallel for 
			//for (int i=i2;i < sizer;i++) { 
			//    data2[i + i2o] = datar[i];
			//}
		}
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

void msort(dataType *data, int size, dataType *data2){
	if (size>1){
		int siz = size>>1;
		int siz1 = (size+1)>>1;
		{	
			{
				#pragma omp task if (size > 1<<20)
				{
					msort(data2,siz,data);
				}
				#pragma omp task if (size > 1 <<20)
				{
					msort(data2+siz, siz1, data+siz);
				}
			}
		}

		#pragma omp taskwait
		
		{
			merge(data2,siz,data2+siz, siz1,data);
		}
	}
}

void mergesort(dataType *data, int size){
	dataType* data2=(dataType*)malloc(size*sizeof(dataType));
	if (data2==0){
		cout<<"OUT OF MEMORY\n";
		exit(0);
	}
	#pragma omp parallel   
	{
		//omp_set_nested(1);
		#pragma omp for 
		for (int i=0; i<size; i++){
			data2[i] = data[i];
		}

		#pragma omp master
		{
			msort(data, size, data2);	
		}
	}
	free(data2);
}


