/*
 * active.c
 *
 * Neuron activation functions
 */

#include "inc/active.h"

/*
 * ReLU activation function
 *
 * in = Function input
 *
 * Returns ReLU output
 */
float active_relu(float in)
{
	return (in > 0 ? in : 0);
}

/*
 * ReLU derivative function
 *
 * in = Function in
 *
 * Returns ReLU derivative function
 */
float active_relu_der(float in)
{
	return (in > 0 ? 1 : 0);
}