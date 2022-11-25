/*
 * It should be noted that matrix values arrays are addressed as follows:
 *
 * l->values[ROW][COL] or l->values[Y][X]
 */

#ifndef MATRIX_H
#define MATRIX_H

/* Types and structs */
// Matrix struct
typedef struct matrix {
	int width;
	int height;
	float **values;
} matrix_t;

/* Prototypes */
void matrix_mul(matrix_t *a, matrix_t *b, matrix_t *c);
matrix_t *matrix_nmul(matrix_t *a, matrix_t *b);
void matrix_add(matrix_t *a, matrix_t *b, matrix_t *c);
matrix_t *matrix_nadd(matrix_t *a, matrix_t *b);
void matrix_sub(matrix_t *a, matrix_t *b, matrix_t *c);
matrix_t *matrix_nsub(matrix_t *a, matrix_t *b);
void matrix_trans(matrix_t *a, matrix_t *b);
matrix_t *matrix_ntrans(matrix_t *a);
void matrix_prod(matrix_t *a, matrix_t *b, matrix_t *c);
matrix_t *matrix_nprod(matrix_t *a, matrix_t *b);

matrix_t *matrix_new(int width, int height);
void matrix_free(matrix_t *m);
void matrix_print(matrix_t *m);

#endif