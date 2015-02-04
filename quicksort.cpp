#include <iostream>
#include <omp.h>
#include "quicksort.h"


using namespace std;

void swap(dataType *data, int left, int right){
	dataType temp = data[left];
	data[left] = data[right];
	data[right] = temp;
}

void quicksort(dataType *data, int start, int end){
	if (start+1<end){
		cout<<start<<" "<<end<<"IN"<<endl;
		int left, right;
		int sorted = 1;
	    for(left = start+1, right = end-1; left < right; ){
	        if(data[left].key > data[start].key && data[right].key <= data[start].key){
	            swap(data, left, right);
				sorted = 0;
	        }
	        if(data[left].key <= data[start].key) left++;
	        if(data[right].key > data[start].key) right--;
	    }
	    // swap(data, start, --left);

		// if (!sorted){
			//#pragma omp parallel num_threads(2)
			{
				//if (omp_get_thread_num()==0){
					quicksort(data, start, left);
				//}else{
					quicksort(data, right, end);
				//}
			}
		// }	
	}else{
		cout<<start<<" "<<end<<endl;
	}

}
