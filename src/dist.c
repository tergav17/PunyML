/*
 * dist.c
 *
 * Statistical distribution generation + random numbers
 */

#include "inc/dist.h"

#include <math.h>
#include <stdlib.h>
#include <time.h>

void dist_init()
{
	srand(time(NULL));
}

/*
 * Returns a random float ranging from 0.0 to 1.0
 */
float dist_randf()
{
	return ((float) rand()) / RAND_MAX;
}

/*
 * Generates a random gaussian pair
 *
 * Method borrowed from numpy
 *
 * Return a f_pair_t with generated values
 */
f_pair_t dist_gauss()
{
	double x1, x2, r, f;
	f_pair_t p;
	
	do {
		x1 = 2.0F * dist_randf() - 1.0;
		x2 = 2.0F * dist_randf() - 1.0;
		r = (x1 * x1) + (x2 * x2);
	} while (r >= 1.00 || r == 0.0);
	
	f = sqrt(-2.0 * log(r) / r);
	
	p.f1 = f * x1;
	p.f2 = f * x2;
	
	return p;
}

/*
 * Places values into an array for he weight initialization
 *
 * a = Pointer to array
 * cnt = Number of values to generate
 * in = Inputs per node
 */
void dist_he_fill(float *a, int cnt, int in)
{
	
	f_pair_t p;
	int i;
	float f, s;
	
	// Scale value
	s = sqrt(2.0 / in);
	
	for (i = 0; i < cnt; i++) {
		// Get gaussian value
		if (i%2 == 0) {
			p = dist_gauss();
			f = p.f1;
		} else {
			f = p.f2;
		}
		
		// Place in array and increment
		*a++ = f * s;
	}
}