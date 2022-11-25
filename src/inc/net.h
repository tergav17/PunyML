#ifndef NET_H
#define NET_H

#include "matrix.h"
#include "layer.h"
#include "net.h"

/* Types and structs */

// Neural network data structure
typedef struct network {
	layer_t *layer_head;
	layer_t *layer_tail;
	int depth;
	
	int isize;
	int osize;
} network_t;

/* Prototypes */
matrix_t *net_execute(network_t *net, matrix_t *in);
void net_add_layer(network_t *net, int size, actf_t act, actf_t der, initf_t init);
network_t *net_new(int size);
void net_free(network_t *n);

#endif