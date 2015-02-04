#include "payloadsize.h"  // Defines LOADSIZE

typedef enum {
	QUICK,
	MERGE,
	RADIX,
	BEST
}  SortType;

typedef struct {
   long long *key;
   char payload[LOADSIZE];
} dataType;

void pSort(dataType *data, int ndata, SortType sorter=BEST);
