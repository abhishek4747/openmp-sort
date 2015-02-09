#include <iostream>
#include <omp.h>
#include "quicksort.h"


using namespace std;

void swap(dataType *data, int left, int right){
	dataType temp = data[left];
	data[left] = data[right];
	data[right] = temp;
}

int partition(dataType *data, int start, int end){
	int pi = end-1;
	int store = start;
	for (int i = start; i<end-1; i++){
		if (data[i].key<=data[pi].key){
			swap(data, i, store);
			store++;
		}
	}	
	swap(data, pi, store);
	return store;

}

void quicksort(dataType *data, int start, int end){
	if (start+2==end){
		if (data[start].key>data[start+1].key){
			swap(data, start, start+1);
		}
	}
	else if (start+1<end){
		/*for	(int i = start; i<end; i++){
			cout<<(long long)data[i].key<<", ";
		}
		cout<<endl;*/
		int pivot = partition(data, start, end);
		/*
		//cout<<start<<" "<<end<<"IN"<<endl;
		int left, right;
		int sorted = 1;
	    for(left = start+1, right = end-1; left < right; ){
			while (left<end && data[left].key <= data[start].key)
				left++;
			while (start<right && data[right].key > data[start].key)
				right--;
	        if(left<right && data[left].key > data[start].key && data[right].key <= data[start].key){
//	            cout<<(long long)data[left].key<<"dd -- "<<(long long)data[right].key<<endl;
				swap(data, left, right);
//	            cout<<(long long)data[left].key<<" -- "<<(long long)data[right].key<<endl<<endl;
				sorted = 0;
	        }

	    }
		
for	(int i = start; i<end; i++){
			cout<<(long long)data[i].key<<", ";
		}
		cout<<endl;
			
		if (right!=end-1){
			//++right;
		}
		if (left!=start+1){
			//--left;
	    	swap(data, start, --left);
		}
		*/
		/*
		cout<<"pivot: "<<pivot-start<<" \t";
		for	(int i = start; i<end; i++){
			cout<<(long long)data[i].key<<", ";
		}
		cout<<endl<<endl;
		*/
		if (start+1<pivot)
			quicksort(data, start, pivot);
		if (pivot+2<end)
			quicksort(data, pivot+1, end);
		// if (!sorted){
			//#pragma omp parallel num_threads(2)
			{
				//if (omp_get_thread_num()==0){
				//}else{
				//}
			}
		// }	
	}else{
		//cout<<start<<" "<<end<<endl;
	}

}
