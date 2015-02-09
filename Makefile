T = 10
P = 1
S = q

G_ARGS = -fPIC -c -g  -Wall

all:
	echo "try 'make test'"

bquick: quicksort.cpp
	g++ -fopenmp $(G_ARGS) quicksort.cpp 

bmerge: mergesort.cpp
	g++ -fopenmp $(G_ARGS) mergesort.cpp 

bradix: radixsort.cpp
	g++ -fopenmp $(G_ARGS) radixsort.cpp 

bother: othersort.cpp
	g++ -fopenmp $(G_ARGS) othersort.cpp 

bsort: sort.cpp
	g++ -fopenmp $(G_ARGS) sort.cpp

ball: bquick bmerge bradix bother bsort

blib: ball
	g++ -shared -o libpsort.so quicksort.o mergesort.o radixsort.o othersort.o sort.o

btest: blib test.cpp 
	g++ -fopenmp test.cpp -o test -L. -lpsort

test: btest
	LD_LIBRARY_PATH=. ./test $(T) $(P) $(S)

clean:
	rm -f *.o
	rm -f *.so
	clear
