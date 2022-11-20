/*
 * layer.c
 *
 * Layer initalization and management
 */

#include "inc/layer.h"

#include <stdlib.h>

/*
 * Given a layer struct and initalization function,
 * this function will initalize the weights and bias in a layer
 *
 * l = Layer to initalize
 * init = Initalization function
 */
void layer_init(layer_t *l, initf_t init)
{
	int i;
	
	// Zero out the bias matrix
	for (i = 0; i < l->bias->height; i++)
		l->bias->values[i][0] = 0;
	
	// And init the weight matrix
	for (i = 0; i < l->weight->height; i++)
		init(l->weight->values[i], l->weight->width, l->isize);
}

/*
 * Update the result given the result from the last matrix
 *
 * l = Layer to execute
 */
void layer_execute(layer_t *l, matrix_t *prev)
{
	int i;
	
	// If there is no matrix or no previous matrix, bad news!
	if (!l || !prev) return;
	
	// Multiply the weight by the results of the last layer
	matrix_mul(l->weight, prev, l->result);
	
	// Add the bias
	matrix_add(l->result, l->bias, l->result);
	
	// Now run everything through the activation function
	for (i = 0; i < l->result->height; i++)
		l->result->values[i][0] = l->act(l->result->values[i][0]);
}

/*
 * Allocates memory for a new layer struct
 * Matrix structs are automatically generated based on size inputs
 *
 * isize = Input layer size
 * osize = Output layer size
 * act = Activation function
 * der = Derivative of activation function
 *
 * Returns pointer to new layer struct
 */
layer_t *layer_new(int isize, int osize, actf_t act, actf_t der)
{
	layer_t *new;
	
	// Alloc new struct
	new = (layer_t *) malloc(sizeof(layer_t));
	
	// Set activate function and derivative
	new->act = act;
	new->der = der;
	
	// Create weight, bias, cache matrix
	new->weight = matrix_new(isize, osize);
	new->bias = matrix_new(1, osize);
	new->result = matrix_new(1, osize);
	
	// Set the input and output sizes
	new->isize = isize;
	new->osize = osize;
	
	// Null out next and prev layer
	new->next = NULL;
	new->prev = NULL;

	return new;
}

/*
 * Frees the utilized memory of an existing layer struct
 *
 * l = Layer struct
 */
void layer_free(layer_t *l)
{
	// Free matrixes
	matrix_free(l->weight);
	matrix_free(l->bias);
	matrix_free(l->result);
	
	// Free struct
	free(l);
}