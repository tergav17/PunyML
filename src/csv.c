/*
 * csv.c
 *
 * Functions for loading training records from csv files
 */

#include "inc/csv.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Loads a batch of training data from a csv file into memory
 *
 * path = Path a csv file
 * max = Maximum integer size (usually 256)
 * single = Singleton label mode
 * isize = Input size
 * osize = Output side
 */
batch_t *csv_load(char *path, int max, char single, int isize, int osize)
{
	FILE *f;
	batch_t *batch;
	char buf[512], tmp[32], reading;
	int i, o, k, row, col, len, cell, records;
	
	printf("Reading from file %s\n", path);
	
	f = fopen(path, "rb");
	
	if (f) {
		printf("File successfully opened, indexing...\n");
	} else {
		printf("Failed to open file!\n");
		return NULL;
	}
	
	// Time to count records
	records = 0;
	reading = 0;
	// Read file in blocks for extra speedy
	// Not sure if this actually helps things outside of V6 UNIX, but whateves
	while ((len = fread(buf, 1, 512, f))) {
		// Iterate through read characters
		for (i = 0; i < len; i++) {
			if (buf[i] != '\n') {
				if (!reading) {
					reading = 1;
					records++;
				}
			} else {
				// Indicate start of new line
				reading = 0;
			}
		}
	}
	
	printf("Indexed %d records to read\n", records);
	
	// Create datastructure
	batch = (batch_t *) malloc(sizeof(batch_t));
	batch->samples = (sample_t **) malloc(sizeof(sample_t *) * records);
	for (i = 0; i < records; i++) {
		batch->samples[i] = csv_sample_new(isize, osize);
	}
	batch->count = records;
	
	// Now we actually load in the records
	rewind(f);
	o = col = row = 0;
	while ((len = fread(buf, 1, 512, f))) {
		// When we have read enough records, break
		
		
		// Iterate through read characters
		for (i = 0; i < len; i++) {
			if (row >= records) break;
			
			if (buf[i] == ',' || buf[i] == '\n') {
				// Zero terminate
				tmp[o] = '\0';
				
				// Get value of cell
				cell = atoi(tmp);
				if (!o) cell = 0;
				
				if (col >= isize + (single ? 1 : osize)) {
					printf("Too many collumns for record %d!\n", row);
					return NULL;
				}
				
				batch->samples[row]->serial = row;
				
				if (single) {
					if (col) {
						// Cell in input matrix
						batch->samples[row]->input->values[col-1][0] = ((float) cell) / ((float) max);
					} else {
						// Cell in output matrix
						for (k = 0; k < batch->samples[row]->output->height; k++) {
							batch->samples[row]->output->values[k][0] = (k == cell) ? 1.0 : 0.0;
						}
					}
				} else {
					if (col < osize) {
						// Cell in output matrix
						batch->samples[row]->output->values[col][0] = ((float) cell) / ((float) max);
					} else {
						// Cell in input matrix
						batch->samples[row]->input->values[col-osize][0] = ((float) cell) / ((float) max);
					}
				}
				
				// Next collumn
				col++;
				
				// Line break sanity checking
				if (buf[i] == '\n') {
					if (col != isize + (single ? 1 : osize)) {
						printf("Not enough collumns for record %d!\n", row);
						return NULL;
					}
					row++;
					col = 0;
				}
				
				o = 0;
				
			} else {
				if (o < 31) tmp[o++] = buf[i];
			}
		}
	}
	
	// All done
	fclose(f);
	
	
	printf("Samples successfully read from file\n");
	return (batch_t *) batch;
}

/*
 * Creates a new batch struct using random samples for a source struct
 * The samples found in the new struct are pointers to the one in the source struct
 *
 * source = Source batch struct
 * count = Number of samples in the output
 */
batch_t *csv_subset(batch_t *source, int count)
{
	batch_t *new;
	int i;
	
	// Create datastructure
	new = (batch_t *) malloc(sizeof(batch_t));
	new->samples = (sample_t **) malloc(sizeof(sample_t *) * count);

	// Select random samples from source
	for (i = 0; i < count; i++)
		new->samples[i] = source->samples[rand() % source->count];

	// Set the count
	new->count = count;

	return new;
}

/*
 * Creates a new empty sample struct
 *
 * isize = Input size
 * osize = Output size
 *
 * Returns newly created sample struct 
 */
sample_t *csv_sample_new(int isize, int osize)
{
	sample_t *new;
	
	// Allocate memory for struct
	new = (sample_t *) malloc(sizeof(sample_t));
	
	// Create submatricies
	new->input = matrix_new(1, isize);
	new->output = matrix_new(1, osize);
	
	return new;
}

/*
 * Frees a sample struct (including matricies)
 *
 * sample = Pointer to sample struct
 */
void csv_sample_free(sample_t *sample) {
	// Free matricies
	matrix_free(sample->input);
	matrix_free(sample->output);
	
	// Free struct
	free(sample);
}


/*
 * Frees a batch struct without freeing the stored samples
 *
 * batch = Pointer to batch struct
 */
void csv_batch_free(batch_t *batch) {
	// Free struct
	free(batch->samples);
	free(batch);
}

/*
 * Frees a batch struct (including samples)
 *
 * batch = Pointer to batch struct
 */
void csv_batch_free_all(batch_t *batch) {
	int i;
	
	// Free all samples
	for (i = 0; i < batch->count; i++)
		csv_sample_free(batch->samples[i]);
	
	// Free batch
	csv_batch_free(batch);
}