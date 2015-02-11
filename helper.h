#ifndef LOADSIZE
	#include "sort.h"
#endif

#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define getkey(d,i) ((long long)d[i].key)

#define swap(d,a,b) {\
	dataType dt = d[b]; \
	d[b] = d[a]; \
	d[a] = dt;  \
	}

