#include <stdio.h>
#include "inc/dist.h"
#include "inc/matrix.h"

int main()
{
	matrix_t *a,*b;
	
	// Init random seed to time
	dist_init();
	
	a = matrix_new(2,2);
	a->values[0][0] = 1;
	a->values[0][1] = 2;
	a->values[1][0] = 3;
	a->values[1][1] = 4;


	b = matrix_new(2,2);
	b->values[0][0] = 7;
	b->values[0][1] = 8;
	b->values[1][0] = 9;
	b->values[1][1] = 10;
	
	matrix_print(matrix_nsub(a,b));
}
