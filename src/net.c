/*
 * net.c
 *
 * Neural network guts
 */
 
#include "inc/net.h"

#include <stdlib.h>

/*
 * Feeds a set of inputs into the network, and returns the result
 *
 * net = Network to execute
 * in = Input layer
 *
 * Returns pointer to output matrix (do not try to free)
 */
matrix_t *net_execute(network_t *net, matrix_t *in)
{
	layer_t *curr_layer;
	matrix_t *result;
	
	// Make sure input matrix matches bounds
	// I know, actual sanity checking!
	if (in->width != 1 || in->height != net->isize) return NULL;
	
	// If the network has no layers, just return the input matrix
	if (!net->layer_head) return in;
	curr_layer = net->layer_head;
	
	// Feed the first input matrix into the network
	layer_execute(curr_layer, in);
	result = curr_layer->result;
	
	// Now do all the next layers
	while (curr_layer->next) {
		// Execute the next layer on this layer's results
		layer_execute(curr_layer->next, result);
		
		// Advance to next layer
		result = curr_layer->result;
		curr_layer = curr_layer->next;
	}
	
	// And we are done, so easy!
	return result;
}

void net_add_layer(network_t *net, int size, actf_t act, actf_t der, initf_t init)
{
	layer_t *new;
	
	// Make sure network actually exists
	if (!net) return;
	
	// Create new layer
	// Input size is the size of the last output
	// Output size is the defined size
	new = layer_new(net->osize, size, act, der);
	
	// Update depth and net output size
	net->depth++;
	net->osize = size;
	
	// Now append the layer onto the doubly linked list
	if (!net->layer_head)
		net->layer_head = new;
	
	if (!net->layer_tail)
		net->layer_tail = new;
	else {
		new->prev = net->layer_tail;
		net->layer_tail->next = new;
		net->layer_tail = new;
	}
	
	// Finally, init the new layer
	layer_init(new, init);
}

/*
 * Creates a new network struct
 *
 * size = Size of input
 *
 * Returns new network struct
 */
network_t *net_new(int size)
{
	network_t *new;
	
	// Allocate memory for new struct
	new = (network_t *) malloc(sizeof(network_t));
	
	// Set input and output size
	// This will be the same as the network has no layers
	new->isize = size;
	new->osize = size;
	
	// Set up linked list stuff too
	new->depth = 0;
	new->layer_head = NULL;
	new->layer_tail = NULL;
	
	return new;
}

/*
 * Frees network struct and all attached layers
 *
 * n = Network struct
 */
void net_free(network_t *n)
{
	layer_t *curr_layer, *next_layer;
	
	// Free all attached layers
	curr_layer = n->layer_head;
	while (curr_layer) {
		next_layer = curr_layer->next;
		layer_free(curr_layer);
		curr_layer = next_layer;
	}
	
	// Free struct
	free(n);
}