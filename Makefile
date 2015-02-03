TWO_POW = 10

all: bquick bmerge bradix bother

bquick: quicksort.c
	gcc quicksort.c -o quicksort.o 

bmerge: mergesort.c
	gcc mergesort.c -o mergesort.o

bradix: radixsort.c
	gcc radixsort.c -o radixsort.o

bother: othersort.c
	gcc othersort.c -o othersort.o

yo:
	@echo "yo $(TWO_POW)"

quick: bquick
	./quicksort.o $(TWO_POW)

merge: bmerge
	./mergesort.o $(TWO_POW)

radix: bradix
	./radixsort.o $(TWO_POW)

other: bother
	./othersort.o $(TWO_POW)

clean:
	rm *.o