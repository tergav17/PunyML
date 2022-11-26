#ifndef CSV_H
#define CSV_H

#include "matrix.h"

/* Types and structs */
// Training sample struct
typedef struct sample {
	matrix_t *input;
	matrix_t *output;
	
	int serial;			// Serial number for training object
} sample_t;

typedef struct batch {
	sample_t **samples;
	
	int count;
} batch_t;

/* Prototypes */
batch_t *csv_load(char *path, int max, char single, int isize, int osize);
batch_t *csv_subset(batch_t *source, int count);
sample_t *csv_sample_new(int isize, int osize);
void csv_sample_free(sample_t *sample);
void csv_batch_free(batch_t *batch);
void csv_batch_free_all(batch_t *batch);

#endif