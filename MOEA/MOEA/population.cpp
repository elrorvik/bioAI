#include<iostream>
#include"population.h"
#include"parameters.h"

Population::~Population() {

}

void Population::Population(int res_height, int res_width, int **pixels) {
	population = new node**[N_IND];
	for (int ind_i = 0; ind_i < NPOP; ind_i++) {
		population[ind_i] = new node*[res_height];
		for (int row_i = 0; row_i < res_height; row_i++) {
			population[ind_i][row_i] = new node[res_width]{};
		}
	}
	image = new RGB*[res_height];
	for (int row_i = 0; row_i < res_height; row_i++) {
		image[row_i] = new RGB[res_width];
	}
}

void Population::initialize_k_means() {
	// Kruskal's =~ Prim's?
	// Remove the two nodes with least edge weight (weight = "cost" = RGB distance) and add it to its own tree

	
}