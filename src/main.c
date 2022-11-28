#include <stdio.h>
#include "inc/layer.h"
#include "inc/matrix.h"
#include "inc/dist.h"
#include "inc/net.h"
#include "inc/active.h"
#include "inc/csv.h"
#include "inc/train.h"

int main()
{
	batch_t *tset, *sset;
	layer_t *l;
	network_t *net;
	int i,j ;
	
	// Init random seeds
	dist_init();
	
	tset = csv_load("mnist_test.csv", 256, 1, 784, 10);
	
	net = net_new(784);
	
	net_add_layer(net, 30, &active_relu, &active_relu_der, &dist_he_init);
	net_add_layer(net, 10, &active_relu, &active_relu_der, &dist_he_init);
	
	printf("Network stats: Depth=%d, ISize=%d, OSize=%d\n", net->depth, net->isize, net->osize);
	//net_execute(net, tset->samples[0]->input);
	//matrix_print(net->layer_tail->result);
	//printf("Error: %f\n", train_cost(net_execute(net, tset->samples[0]->input), tset->samples[0]->output));
	
	//if (net) return 0;
	
	printf("Initial performance: %d/%d correct\n", train_correct(net, tset), tset->count);
	
	for (j = 0; j < 30; j++) {
		printf("Starting epoch #%d at cost %f...\n", j, train_cost_batch(net, tset));	
	
		for (i = 0; i < tset->count/10; i++) {
			sset = csv_subset(tset, 10);
		
			// Do the training
			train_batch(net, sset, 0.05);	
		}
		
		printf("End epoch #%d at cost %f (%d/%d correct)\n", j, train_cost_batch(net, tset), train_correct(net, tset), tset->count);
	}
	
	// Print out network
	l = net->layer_head;
	for (i = 0; i < net->depth; i++) {
		printf("Layer #%d Weights\n", i+1);
		matrix_print(l->weight);
		printf("Layer #%d Bias\n", i+1);
		matrix_print(l->bias);
	}
	
}
