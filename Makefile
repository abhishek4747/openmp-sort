T = 10
P = 1
S = q

G_ARGS = -fopenmp -fPIC -c -g  -Wall

all:
	echo "try 'make test'"

bhelper: helper.cpp
	@g++ $(G_ARGS) helper.cpp

bpsum: psum.cpp
	@g++ $(G_ARGS) psum.cpp

bquick: quicksort.cpp
	@g++ $(G_ARGS) quicksort.cpp 

bmerge: mergesort.cpp
	@g++ $(G_ARGS) mergesort.cpp 

bradix: radixsort.cpp
	@g++ $(G_ARGS) radixsort.cpp 

bother: othersort.cpp
	@g++ $(G_ARGS) othersort.cpp 

bsort: sort.cpp
	@g++ $(G_ARGS) sort.cpp

ball: bquick bmerge bradix bother bsort bpsum bhelper

blib: ball
	@g++ -shared -o libpsort.so quicksort.o mergesort.o radixsort.o othersort.o sort.o psum.o helper.o

btest: blib test.cpp 
	@g++ -fopenmp test.cpp -o test -L. -lpsort

test: btest
	@LD_LIBRARY_PATH=. ./test $(T) $(P) $(S)

clean:
	rm -f *.o
	rm -f *.so
	rm test
	clear
