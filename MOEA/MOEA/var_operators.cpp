#include "var_operators.h"
#include "global.h"
#include "population.h"

std::vector<active_edge_t> crossover_uniform_list_representation(Population &p, int parent_A, int parent_B) {
	std::vector<active_edge_t> parent_A_chromosome = p.get_edge_candidates(parent_A);
	std::vector<active_edge_t> parent_B_chromosome = p.get_edge_candidates(parent_B);

	std::vector<active_edge_t> offspring_chromosome;
	for (int gene_index = 0; gene_index < parent_A_chromosome.size(); gene_index++) {
		bool from_A = rand() % 2;
		if (from_A) offspring_chromosome[gene_index] = parent_A_chromosome[gene_index];
		else offspring_chromosome[gene_index] = parent_B_chromosome[gene_index];
	}
	return offspring_chromosome;
}
/*
std::vector<int> mutation_flip_list_representation() {

}
std::vector<int> mutation_set_list_representation() {

}

std::vector<int> mutation_reset_list_representation() {

}*/