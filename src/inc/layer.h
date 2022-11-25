#ifndef LAYER_H
#define LAYER_H

#include "matrix.h"

/* Types and structs */
// Function type for activation functions
typedef float (*actf_t)(float);

// Function type for initialization functions
typedef void  (*initf_t)(float *, int, int);

//Individual neural network weight layer
typedef struct layer {
	matrix_t *weight;
	matrix_t *bias;
	
	int isize;			// Layer input size
	int osize;			// Layer output size
	
	actf_t act;			// Activation function
	actf_t der;			// Derivative of activation function
	
	matrix_t *z;		// Calculate intermediate
	matrix_t *result;	// Calculation result
	
	struct layer *next;	// Next layer, if available
	struct layer *prev;	// Previous layer, if available
} layer_t;

/* Prototypes */
void layer_init(layer_t *l, initf_t init);
void layer_execute(layer_t *l, matrix_t *prev);
layer_t *layer_new(int isize, int osize, actf_t act, actf_t der);
void layer_free(layer_t *l);

#endif