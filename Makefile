T = 10

all: bquick bmerge bradix bother

bquick: quicksort.cpp
	@g++ -fopenmp quicksort.cpp -o quicksort.o 

bmerge: mergesort.cpp
	@g++ -fopenmp mergesort.cpp -o mergesort.o

bradix: radixsort.cpp
	@g++ -fopenmp radixsort.cpp -o radixsort.o

bother: othersort.cpp
	@g++ -fopenmp othersort.cpp -o othersort.o

yo:
	@echo "yo $(T)"

quick: bquick
	@./quicksort.o $(T) $(P)

merge: bmerge
	@./mergesort.o $(T) $(P)

radix: bradix
	@./radixsort.o $(T) $(P)

other: bother
	@./othersort.o $(T) $(P)

clean:
	rm *.o