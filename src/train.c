/*
 * train.c
 *
 * Network training functions
 */

#include "inc/train.h"

#include <stdlib.h>
#include <stdio.h>

/*
 * Calculates the cost value for a result and desired outcome
 *
 * res = Result matrix
 * des = Desired outcome matrix
 *
 * Returns cost value
 */
float train_cost(matrix_t *res, matrix_t *des)
{
	int i;
	float tmp, cost;
	
	// Summate the squared differences between result and desired outcome *phew*
	cost = 0;
	for (i = 0; i < res->height; i++) {
		tmp = res->values[i][0] - des->values[i][0];
		cost += tmp * tmp;
	}
	
	return cost;
}

/*
 * Feeds forwards the network through multiple sample and returns an average cost
 *
 * net = Neural network struct
 * batch = Batch of samples
 */
float train_cost_batch(network_t *net, batch_t *batch)
{
	int i;
	float cost;
	
	cost = 0;
	for (i = 0; i < batch->count; i++) {
		cost += train_cost(net_execute(net, batch->samples[i]->input), batch->samples[i]->output);
	}
	cost /= (float) batch->count;
	
	return cost;
}

/*
 * Calculates the derivative of the cost value, returned in matrix cost
 *
 * res = Result matrix
 * des = Desired outcome matrix
 * cost = Derivative of cost matrix
 */
void train_cost_d(matrix_t *res, matrix_t *des, matrix_t *cost)
{
	matrix_sub(res, des, cost);
}


int train_correct(network_t *net, batch_t *batch)
{
	int i, j, max_index, correct;
	matrix_t *activations;
	float max_act;
	
	// Start correct count at 0
	correct = 0;
	
	// Check each batch
	for (i = 0; i < batch->count; i++) {
		max_act = -1.0;
		max_index = -1;
		
		activations = net_execute(net, batch->samples[i]->input);
		
		// Search for the output index with the largest
		for (j = 0; j < activations->height; j++) {
			if (activations->values[j][0] > max_act) {
				max_act = activations->values[j][0];
				max_index = j;
			}
		}
		
		// See if it is the correct result from the sample
		if (batch->samples[i]->output->values[max_index][0] > 0.99) correct++;
	}
	
	return correct;
}


/*
 * Updates weights in a network based on a batch of training data
 *
 * net = Pointer to neural network struct
 * batch = Pointer to training set
 * rate = Learning rate
 */
void train_batch(network_t *net, batch_t *batch, float rate)
{
	int i, x, y;
	float m;
	layer_t *l;
	matrix_t **grad_w, **grad_b;
	
	/*
	 * Here are some global registers so we don't have to keep allocating
	 * and deallocating memory all the time
	 */
	matrix_t **delta_w, **delta_b;
	
	// First order of business is to set up the gradient matricies 
	grad_w = (matrix_t **) malloc(sizeof(matrix_t *) * net->depth);
	grad_b = (matrix_t **) malloc(sizeof(matrix_t *) * net->depth);
	
	// Lets set up the registers too while we are here
	delta_w = (matrix_t **) malloc(sizeof(matrix_t *) * net->depth);
	delta_b = (matrix_t **) malloc(sizeof(matrix_t *) * net->depth);
	
	// Create a zeroed matrix for every weight and bias gradient
	l = net->layer_head;
	i = 0;
	while (l) {
		
		grad_w[i] = matrix_new(l->weight->width, l->weight->height);
		grad_b[i] = matrix_new(1, l->bias->height);
		
		// Lets zero them both out
		for (y = 0; y < l->weight->height; y++)
			for (x = 0; x < l->weight->width; x++)
				grad_w[i]->values[y][x] = 0.0;
		
		for (y = 0; y < l->bias->height; y++)
			grad_b[i]->values[y][0] = 0.0;
		
		// Do the registers too
		delta_w[i] = matrix_new(l->weight->width, l->weight->height);
		delta_b[i] = matrix_new(1, l->bias->height);
		
		// Next layer
		i++;
		l = l->next;
	}
	
	// Now we can run back propigation on all of the training samples
	for (i = 0; i < batch->count; i++)
		train_backprop(net, batch->samples[i], grad_w, grad_b, delta_w, delta_b);
	
	// Finally, update the weights and bias
	m = rate / ((float) batch->count);
	
	// Iterate through the different layers and update
	l = net->layer_head;
	i = 0;
	while (l) {
		// Update weights
		for (y = 0; y < l->weight->height; y++)
			for (x = 0; x < l->weight->width; x++)
				l->weight->values[y][x] -= m * grad_w[i]->values[y][x];
			
		// Update bias
		for (y = 0; y < l->bias->height; y++)
			l->bias->values[y][0] -= m * grad_b[i]->values[y][0];
		
		// Next layer
		i++;
		l = l->next;
	}
	
	// Free gradient and regsiter matricies
	for (i = 0; i < net->depth; i++) {
		matrix_free(grad_w[i]);
		matrix_free(grad_b[i]);
		matrix_free(delta_w[i]);
		matrix_free(delta_b[i]);
	}
	free(grad_w);
	free(grad_b);
	free(delta_w);
	free(delta_b);
	
	// All done
}

/*
 * Runs back propigation on the network given a single sample
 * Results are added to grad_w and grad_b
 *
 * net = Pointer to neural network struct
 * sample = Pointer to training sample
 * grad_w = Pointer to weight gradients
 * grad_b = Pointer to bias gradients
 * delta_w = Weights delta (same dimensions as grad_w)
 * delta_b = Bias delta (same dimensions as grad_b)
 */
void train_backprop(network_t *net, sample_t *sample, matrix_t **grad_w, matrix_t **grad_b, matrix_t **delta_w, matrix_t **delta_b)
{
	int i, y, depth;
	matrix_t *act, *wet;
	layer_t *l;
	
	// Sanity check for training sample
	if (sample->input->height != net->isize || sample->input->width != 1) {
		printf("	Input mismatch of sample record #%d! Sample=%dx%d, Network=%d\n", sample->serial, sample->input->width, sample->input->height, net->isize);
		return;
	}
	if (sample->output->height != net->osize || sample->output->width != 1) {
		printf("	Output mismatch of sample record #%d! Sample=%dx%d, Network=%d\n", sample->serial, sample->output->width, sample->output->height, net->osize);
		return;
	}
	
	// Feed forward the training sample
	net_execute(net, sample->input);
	
	// Record network depth
	depth = net->depth;
	
	// Sanity check
	if (depth <= 0)
		return;
	
	// Start at last layer
	i = depth - 1;
	
	// Calculate BP1
	train_cost_d(net->layer_tail->result, sample->output, delta_b[i]);
	for (y = 0; y < net->osize; y++)
		delta_b[i]->values[y][0] *= net->layer_tail->der(net->layer_tail->z->values[y][0]);
				
	// Add to bias gradient (BP3)
	matrix_add(grad_b[i], delta_b[i], grad_b[i]);
	
	// Get the activation of the previous layer
	if (!i) 
		act = sample->input;
	else
		act = net->layer_tail->prev->result;
	
	// Transpose activation of previous layer
	act = matrix_ntrans(act);
	
	// Now multiply (BP4)
	matrix_mul(delta_b[i], act, delta_w[i]);

	// Free the newly created matrix
	matrix_free(act);
	
	// Add to weight gradient
	matrix_add(grad_w[i], delta_w[i], grad_w[i]);
			
	// Now, we start back propagating
	l = net->layer_tail->prev;
	i--;
	while (l) {
		// Calculate BP2
		// Transpose weights matrix
		wet = matrix_ntrans(l->next->weight);
		
		// Multiply to get delta
		matrix_mul(wet, delta_b[i+1], delta_b[i]);
		
		// Add derivative of activation function 
		for (y = 0; y < l->result->height; y++)
			delta_b[i]->values[y][0] += l->der(l->z->values[y][0]);
		
		// Free newly created matrix
		matrix_free(wet);
		
		// Add to bias gradient (BP3)
		matrix_add(grad_b[i], delta_b[i], grad_b[i]);
		
		// Get the activation of the previous layer
		if (!i)
			act = sample->input;
		else
			act = l->prev->result;
		
		// Transpose activation of previous layer
		act = matrix_ntrans(act);
		
		// Multiply to get delta for layer (BP4)
		matrix_mul(delta_b[i], act, delta_w[i]);
		
		// Free newly created matrix
		matrix_free(act);
		
		// Add to weight gradient
		matrix_add(grad_w[i], delta_w[i], grad_w[i]);
		
		//Onto the next layer
		i--;
		l = l->prev;
	}
	
}