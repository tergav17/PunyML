/*
 * matrix.c
 *
 * Matrix operations and similar routines
 */

#include "inc/matrix.h"

#include <stdlib.h>
#include <stdio.h>

/*
 * Performs the dot product of matrix A and matrix B into matrix C
 * No error checking is performed, caller should already know the bounds of A*B=C
 *
 * a = Pointer to matrix A
 * b = Pointer to matrix B
 * c = Pointer to matrix C
 */
void matrix_mul(matrix_t *a, matrix_t *b, matrix_t *c)
{
	int x,y,z;
	float tmp;
	
	// Checks to see if rows and columns line up
	if (a->width != b->height) printf("Argument mismatch!\n");
	if (c->width != b->width || c->height != a->height) printf("Return mismatch!\n");
	
	for (y = 0; y < c->height; y++) {
		for (x = 0; x < c->width; x++) {
			// Calculate sum of products for this position in the matrix
			tmp = 0;
			for (z = 0; z < a->width; z++) {
				tmp += a->values[y][z] * b->values[z][x];
			}
			
			c->values[y][x] = tmp;
		}
	}
}

/*
 * Performs the dot product of matrix A and matrix B
 * Creates a new matrix struct and stores the result in it
 * Error checking is performed, NULL is returned on error
 *
 * a = Pointer to matrix A
 * b = Pointer to matrix B
 *
 * Returns pointer to matrix C
 */
matrix_t *matrix_nmul(matrix_t *a, matrix_t *b)
{
	matrix_t *c;
	
	// Check to see if both matrix structs exist
	if (!a || !b) return NULL;
	
	// Checks to see if rows and columns line up
	if (a->width != b->height) return NULL;
	
	// Create new matrix and multiply
	c = matrix_new(b->width, a->height);
	matrix_mul(a, b, c);
	
	return c;
	
}


/*
 * Performs the addition of matrix A and matrix B into matrix C
 * No error checking is performed, caller should already know the bounds of A+B=C
 *
 * a = Pointer to matrix A
 * b = Pointer to matrix B
 * c = Pointer to matrix C
 */
void matrix_add(matrix_t *a, matrix_t *b, matrix_t *c)
{
	int x,y;
	
	for (y = 0; y < c->height; y++) {
		for (x = 0; x < c->width; x++) {
			// Calculate sum for this position in the matrix
			c->values[y][x] = a->values[y][x] + b->values[y][x];
		}
	}
}

/*
 * Performs the addition of matrix A and matrix B
 * Creates a new matrix struct and stores the result in it
 * Error checking is performed, NULL is returned on error
 *
 * a = Pointer to matrix A
 * b = Pointer to matrix B
 *
 * Returns pointer to matrix C
 */
matrix_t *matrix_nadd(matrix_t *a, matrix_t *b)
{
	matrix_t *c;
	
	// Check to see if both matrix structs exist
	if (!a || !b) return NULL;
	
	// Checks to see if matrix bounds are the same
	if (a->width != b->width || a->height != b->height) return NULL;
	
	// Create new matrix and add
	c = matrix_new(a->width, a->height);
	matrix_add(a, b, c);
	
	return c;
	
}

/*
 * Performs the subtraction of matrix A and matrix B into matrix C
 * No error checking is performed, caller should already know the bounds of A-B=C
 *
 * a = Pointer to matrix A
 * b = Pointer to matrix B
 * c = Pointer to matrix C
 */
void matrix_sub(matrix_t *a, matrix_t *b, matrix_t *c)
{
	int x,y; 
	
	for (y = 0; y < c->height; y++) {
		for (x = 0; x < c->width; x++) {
			// Calculate minuend for this position in the matrix
			c->values[y][x] = a->values[y][x] - b->values[y][x];
		}
	}
}

/*
 * Performs the subtraction of matrix A and matrix B
 * Creates a new matrix struct and stores the result in it
 * Error checking is performed, NULL is returned on error
 *
 * a = Pointer to matrix A
 * b = Pointer to matrix B
 *
 * Returns pointer to matrix C
 */
matrix_t *matrix_nsub(matrix_t *a, matrix_t *b)
{
	matrix_t *c;
	
	// Check to see if both matrix structs exist
	if (!a || !b) return NULL;
	
	// Checks to see if matrix bounds are the same
	if (a->width != b->width || a->height != b->height) return NULL;
	
	// Create new matrix and subtract
	c = matrix_new(a->width, a->height);
	matrix_sub(a, b, c);
	
	return c;
	
}

/*
 * Peforms a transposition for matrix A into matrix B
 * No error checking is performed, caller should already know the bounds of A^T=B
 *
 * a = Pointer to matrix A
 * b = Pointer to matrix B
 */
void matrix_trans(matrix_t *a, matrix_t *b)
{
	int x,y;
	
	// Just shift the data around
	for (y = 0; y < a->height; y++)
		for (x = 0; x < a->width; x++)
			b->values[x][y] = a->values[y][x];
	
}

/*
 * Performs the transposition of matrix A
 * Creates a new matrix struct and stores the result in it
 * Error checking is performed, NULL is returned on error
 *
 * a = Pointer to matrix A
 *
 * Returns pointer to matrix B
 */
matrix_t *matrix_ntrans(matrix_t *a)
{
	matrix_t *b;
	
	// Make sure that matrix struct exists
	if (!a) return NULL;
	
	// Create new matrix and transpose
	b = matrix_new(a->height, a->width);
	matrix_trans(a, b);
	
	return b;
}


/*
 * Performs a hadamard product of matrix A and matrix B into matrix C
 * No error checking is performed, caller should already know the bounds of A.B=C
 *
 * a = Pointer to matrix A
 * b = Pointer to matrix B
 * c = Pointer to matrix C
 */
void matrix_prod(matrix_t *a, matrix_t *b, matrix_t *c)
{
	int x,y;
	
	// Multiply 'em
	for (y = 0; y < a->height; y++)
		for (x = 0; x < a->width; x++)
			c->values[y][x] = a->values[y][x] * b->values[y][x];
			
}

/*
 * Performs the hadamard product of matrix A and matrix B
 * Creates a new matrix struct and stores the result in it
 * Error checking is performed, NULL is returned on error
 *
 * a = Point to matrix A
 * b = Pointer to matrix B
 *
 * Returns pointer to matrix C
 */
matrix_t *matrix_nprod(matrix_t *a, matrix_t *b)
{
	matrix_t *c;
	
	// Make sure that matrix structs exist
	if (!a || !b) return NULL;
	
	// Checks to see if matrix bounds are the same
	if (a->width != b->width || a->height != b->height) return NULL;
	
	// Create new matrix and produce
	c = matrix_new(a->height, a->width);
	matrix_prod(a, b, c);
	
	return c;
	
}
/*
 * Allocates memory for a new matrix struct
 *
 * width = Width of new matrix
 * height = Height of new matrix
 *
 * Returns pointer to new matrix
 */
matrix_t *matrix_new(int width, int height)
{
	int i;
	matrix_t *new;
	
	// Alloc and setup new struct
	new = (matrix_t *) malloc(sizeof(matrix_t));
	new->width = width;
	new->height = height;
	
	// Allocate memory for values
	new->values = (float **) malloc(sizeof(float *) * height);
	for (i = 0; i < height; i++)
		new->values[i] = (float *) malloc(sizeof(float) * width);
	
	return new;
}

/*
 * Frees the utilized memory of an existing matrix struct
 *
 * m = Pointer to matrix struct
 */
void matrix_free(matrix_t *m)
{
	int i;
	
	//Free rows of matrix
	for (i = 0; i < m->height; i++)
		free(m->values[i]);
	
	//Free matrix itself
	free(m->values);
	
	//Free struct
	free(m);
}

/*
 * Prints out a matrix
 *
 * m = Pointer to matrix struct
 */
void matrix_print(matrix_t *m)
{
	int x,y;
	
	if (!m) {
		printf("[NULL MATRIX]\n");
		return;
	}
	
	for (y = 0; y < m->height; y++) {
		printf("[");
		for (x = 0; x < m->width; x++) {
			printf("%.2f", m->values[y][x]);
			if (x != m->width-1) printf(",	");
		}
		printf("]\n");
	}
}