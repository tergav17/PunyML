#ifndef DIST_H
#define DIST_H

/* Types and structs */
typedef struct f_pair {
	float f1;
	float f2;
} f_pair_t;

/* Prototypes */
void dist_init();
float dist_randf();
f_pair_t dist_gauss();
void dist_he_init(float *a, int cnt, int in); 

#endif