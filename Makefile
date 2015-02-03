T = 10

all: bquick bmerge bradix bother

bquick: quicksort.c
	@gcc quicksort.c -o quicksort.o 

bmerge: mergesort.c
	@gcc mergesort.c -o mergesort.o

bradix: radixsort.c
	@gcc radixsort.c -o radixsort.o

bother: othersort.c
	@gcc othersort.c -o othersort.o

yo:
	@echo "yo $(T)"

quick: bquick
	@./quicksort.o $(T)

merge: bmerge
	@./mergesort.o $(T)

radix: bradix
	@./radixsort.o $(T)

other: bother
	@./othersort.o $(T)

clean:
	rm *.o