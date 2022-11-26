#ifndef TRAIN_H
#define TRAIN_H

#include "matrix.h"
#include "net.h"
#include "csv.h"

/* Prototypes */
float train_cost(matrix_t *res, matrix_t *des);
float train_cost_batch(network_t *net, batch_t *batch);
void train_batch(network_t *net, batch_t *batch, float rate);
void train_backprop(network_t *net, sample_t *sample, matrix_t **grad_w, matrix_t **grad_b, matrix_t **delta_w, matrix_t **delta_b);

#endif