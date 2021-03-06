/************************************************************************
filename: gpu/cuda.cu
author: onesuper
email: onesuperclark@gmail.com

allocate and free memory on device

for common use
*************************************************************************/

#include <stdio.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <cutil.h>


extern Node* d_node_list;
extern Edge* d_edge_list;
extern int* d_color;
extern int* d_cost;
extern int* d_counter;
extern unsigned int* d_current_set_a;    
extern unsigned int* d_current_set_b;
extern int* d_current_set_size_new;
extern unsigned int* current_set;
extern int* current_set_size_new;


bool init_cuda() 
{
	int count = 0;

	cudaGetDeviceCount(&count);
	
	printf("%d device\n", count);

	if (count == 0) {
		fprintf(stderr, "There is no device.\n");
		return false;
	}

	int i;
	for (i=0; i<count; i++) {
		cudaDeviceProp prop;
		if (cudaGetDeviceProperties(&prop, i) == cudaSuccess) {
			if (prop.major >= 1) {
				break;
			}
		}
	}

	if (i == count) {
		fprintf(stderr, "There is no device supporting CUDA 1.x.\n");
		return false;
	}

	cudaSetDevice(i);

	return true;
}


void device_alloc_and_copy(void) {

	 cudaMalloc((void**) &d_node_list, sizeof(Node) * num_of_nodes);
	 cudaMemcpy(d_node_list, node_list, sizeof(Node) * num_of_nodes, cudaMemcpyHostToDevice);

	 cudaMalloc((void**) &d_edge_list, sizeof(Edge) * num_of_edges);
	 cudaMemcpy(d_edge_list, edge_list, sizeof(Edge) * num_of_edges, cudaMemcpyHostToDevice);

	 cudaMalloc((void**) &d_color, sizeof(int) * num_of_nodes);
	 cudaMemcpy(d_color, color, sizeof(int) * num_of_nodes, cudaMemcpyHostToDevice);

	 cudaMalloc((void**) &d_cost, sizeof(int) * num_of_nodes);
	 cudaMemcpy(d_cost, cost, sizeof(int) * num_of_nodes, cudaMemcpyHostToDevice);

	 cudaMalloc((void**) &d_counter, sizeof(int) * MAX_LEVEL);
	 cudaMemcpy(d_counter, counter, sizeof(int) * MAX_LEVEL, cudaMemcpyHostToDevice);

     cudaMalloc((void**) &d_current_set_a, sizeof(unsigned int) * num_of_nodes);
     cudaMemcpy(d_current_set_a, current_set, sizeof(unsigned int) * num_of_nodes, cudaMemcpyHostToDevice);

     cudaMalloc((void**) &d_current_set_b, sizeof(unsigned int) * num_of_nodes);
     cudaMemcpy(d_current_set_b, current_set, sizeof(unsigned int) * num_of_nodes, cudaMemcpyHostToDevice);

     cudaMalloc((void**) &d_current_set_size_new, sizeof(int) * 1);
     cudaMemcpy(d_current_set_size_new, current_set_size_new, sizeof(int) * 1, cudaMemcpyHostToDevice);

	 return; 
}

void device_free(void) {
	 cudaFree(d_node_list);
	 cudaFree(d_edge_list);
	 cudaFree(d_cost);
	 cudaFree(d_color);
	 cudaFree(d_counter);
     cudaFree(d_current_set_a);
     cudaFree(d_current_set_b);
     cudaFree(d_current_set_size_new);
     return;
}
