#ifndef TRAIN_H
#define TRAIN_H

#include "matrix.h"
#include "net.h"

/* Types and structs */
// Training sample struct
typedef struct sample {
	matrix_t *input;
	matrix_t *output;
	
	int serial;			// Serial number for training object
} sample_t;

/* Prototypes */
float train_cost(matrix_t *res, matrix_t *des);
void train_batch(network_t *net, float rate, sample_t *samples, int cnt);
void train_backprop(network_t *net, sample_t *sample, matrix_t **grad_w, matrix_t **grad_b, matrix_t **delta_w, matrix_t **delta_b);

#endif