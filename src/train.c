/*
 * train.c
 *
 * Network training functions
 */

#include "inc/train.h"

/*
 * Calculates the cost value for a result and desired outcome
 *
 * res = Result
 * des = Desired outcome
 *
 * Returns cost value
 */
float train_cost(matrix_t *res, matrix_t *des)
{
	int i;
	float tmp, cost;
	
	// Summate the squared differences between result and desired outcome *phew*
	cost = 0;
	for (i = 0; i < res->height; i++) {
		tmp = res->values[i][0] - des->values[i][0];
		cost += tmp * tmp;
	}
	
	return cost;
}