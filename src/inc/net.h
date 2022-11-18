#ifndef NET_H
#define NET_H

#include "matrix.h"

/* Types and structs */
// 
typedef float (*afunc)(float);

//Individual neural network weight layer
typedef struct layer {
	matrix_t *weight;
	matrix_t *bias;
	
	int isize;
	int osize;
	
	afunc activation;
	afunc derivative;
} layer_t;

// Neural network datastructure
typedef struct network {
	layer_t *layers;
	int lcnt;
	
	matrix_t **cache;
	int ccnt;
	
	int isize;
	int osize;
} network_t;



#endif