#include <stdio.h>
#include "inc/layer.h"
#include "inc/matrix.h"
#include "inc/dist.h"
#include "inc/net.h"
#include "inc/active.h"

int main()
{
	network_t *net;
	matrix_t *input;
	
	// Init random seeds
	dist_init();
	
	net = net_new(2);
	
	net_add_layer(net, 4, &active_relu, &active_relu_der, &dist_he_init);
	net_add_layer(net, 4, &active_relu, &active_relu_der, &dist_he_init);
	
	input = matrix_new(1, 2);
	input->values[0][0] = 1.0;
	input->values[1][0] = 1.0;
	
	matrix_print(net_execute(net, input));
}
