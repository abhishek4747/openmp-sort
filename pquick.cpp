#include "pquick.h"
#include "helper.h"

using namespace std;

// must be called from one thread only
void psum(int *data, int size, int *data2){
	if (size<2)
		return;

	// #1: Pairwise sum
	int s = size>>1;
	#pragma omp parallel for
	for (int i=0; i< s; i++){
		data2[i] = data[2*i] + data[2*i+1];
	}
	// #2: Recurse
	psum(data2, s, data2+s);

	// #3: Finalize
	#pragma omp parallel for
	for (int i=0; i<s; i++){
		data[2*i] = data2[i] -data[2*i+1];
		data[2*i + 1] = data2[i];
	}

	if (size&1){
		data[size-1] = data[size-1] + data[size -2];
	}
}



int splitp ( dataType *a, int upper , long long *pivot){
	int  p, q;
	p =  0 ;
	q = upper - 1 ;

	while ( q >= p ){
		while (q>=p && (long long)a[p].key <= (long long)pivot )
			p++ ;

		while (q>=p && (long long)a[q].key > (long long)pivot )
			q-- ;

		if ( q > p )
			swap(a,p,q);
	}
	return q ;
}
int split ( dataType *a, int upper ){
	int  p, q;
	p =  1 ;
	q = upper - 1 ;

	while ( q >= p ){
		while ((long long) a[p].key < (long long) a[0].key )
			p++ ;

		while ((long long) a[q].key > (long long) a[0].key )
			q-- ;

		if ( q > p )
			swap(a,p,q);
	}
	swap(a,0,q);
	return q ;
}
	
int partition(dataType *data, int size, dataType *data2){
	if (size<2)
		return 0;
	int p = omp_get_num_procs();
	if (size<=(p<<12)){
		return split(data, size);
	}
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
	//int pivot = 0;
	int chunk = ceil(((double)size)/p);
	p = ceil(((double)size)/chunk);

	//cout<<"No. of processors: "<<p<<"\tchunk:"<<chunk<<endl;
	#pragma omp parallel num_threads(p) 
	{
		int i = omp_get_thread_num();
		int start = chunk*i;
		int end = min((int)chunk*(i+1),size);
		/*
		int store = end-1;
		for (int j = start; j<=store;){
			if ((long long)data[j].key>(long long)data[pivot].key){
				swap(data, j, store);
				store--;
			}else{
				j++;
			}
		}
		*/
		int pivot = splitp(data+start, end-start, data[0].key);	
		int store =  start + pivot;
		//cout<<"store:"<<store<<" pivot:"<<pivot<<endl;
		//cout<<i<<" - "<<start<<" -  "<<pivot<<"\n";
		greaterp[i]=end - store - 1;
		lessp[i] = store - start + 1;
	}
	//cout.flush();
	psum(lessp,p,lessp2);
	psum(greaterp,p,greaterp2);
	free(lessp2);
	free(greaterp2);
	/*
	for(int i=0; i<p; i++){
		cout<<lessp[i]<<", ";
	}
	cout<<endl;
	for(int i=0; i<p; i++){
		cout<<greaterp[i]<<", ";
	}
	cout<<endl;*/
/*
	for (int i =0; i<p; i++){
		cout<<lessp[i]<<", ";
	}
	cout<<endl;
	for (int i =0; i<p; i++){
		cout<<greaterp[i]<<", ";
	}
	cout<<endl<<endl;
		
	for (int i =0; i<size; i++){
		cout<<(long long)data[i].key<<", ";
	}
	cout<<endl<<endl;
*/	
	#pragma omp parallel num_threads(p)
	{
		int i = omp_get_thread_num();
		int start = chunk*i;
		//int end = min((int)chunk*(i+1),size);

		int lp = (i>0? lessp[i-1]:0);
		int dp = lessp[i];
		
		for (int j = 0; j<dp-lp; j++){
			data2[lp+j] = data[start+j];
		}
		int gp = (i>0? greaterp[i-1]:0);
		int gd = greaterp[i];
		//cout<<i<<" - "<<lessp[p-1]<<" - "<<gp<<" - "<<gd-gp<<" - "<<dp<<" - "<<start+dp<<endl;
		for (int j =0; j <gd-gp; j++){
			data2[lessp[p-1]+gp+j] = data[start+dp-lp+j];
		}
	}
	
	swap(data2,0,lessp[p-1]-1);	
	
	#pragma omp parallel for num_threads(p)
	for (int i=0; i<size; i++){
		data[i] = data2[i];
	}
	return (lessp[p-1]>0?lessp[p-1]-1:0);
}

void pqsort(dataType *data, int start, int end, dataType *data2){
	if (start+1<end){
		if (end-start==2){
			if ((long long)data[start].key>(long long)data[start+1].key)
				swap(data,start,start+1);
				return;
		}			
		//cout<<start<<" "<<end<<endl;
		/*
		cout<<"pqsort: ";
		for(int i=start; i<end; i++){
			cout<<(long long)data[i].key<<", ";
		}
		cout<<"="<<end-start<<endl;
		*/
		int store = partition(data+start, end-start, data2+start);
		/*
		for(int i=start; i<end; i++){
			cout<<(long long)data[i].key<<", ";
		}
		cout<<":"<<store<<":"<<endl;
		//cout<<start<<" "<<store<<" "<<end<<endl;
		cout<<"pqsortt: ";
		for(int i=start; i<end; i++){
			cout<<(long long)data[i].key<<", ";
		}
		cout<<endl<<endl;
		*/	pqsort(data, start, start+store, data2);
			pqsort(data, start+ store+1, end, data2);
	}
}

void pquicksort(dataType *data, int size){
	dataType *data2;
	data2 = (dataType*) malloc(size*sizeof(dataType));
	if (data2==0){
		cout<<"OUT OF MEMORY"<<endl;
		exit(0);
	}
	pqsort(data, 0, size, data2);
	free(data2);
}

