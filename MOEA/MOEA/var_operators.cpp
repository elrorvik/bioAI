
#include "global.h"
#include "population.h"
#include "var_operators.h"
//#include<vector>


std::vector<active_edge_t> crossover_uniform_list_representation(Population &p, int parent_A, int parent_B, int offspring_index) {
	std::vector<active_edge_t> parent_A_chromosome = p.get_edge_candidates(parent_A);
	std::vector<active_edge_t> parent_B_chromosome = p.get_edge_candidates(parent_B);

	std::vector<active_edge_t> offspring_chromosome = parent_A_chromosome;
	for (int gene_index = 0; gene_index < parent_A_chromosome.size(); gene_index++) {
		bool from_B = rand() % 2;
		if (from_B) {
			offspring_chromosome[gene_index] = parent_B_chromosome[gene_index];
			if (offspring_chromosome[gene_index].active != parent_A_chromosome[gene_index].active) {
				if (offspring_chromosome[gene_index].active) p.merge_segments(offspring_index, gene_index, offspring_chromosome[gene_index].edge);
				else p.split_segment(offspring_index, gene_index, offspring_chromosome[gene_index].edge);
			}
		}
	}
	return offspring_chromosome;
}

std::vector<active_edge_t> crossover_uniform_fluid_list_representation(Population &p, int parent_A, int parent_B, int offspring_index) {
	int crossover_point_seperator_line = rand() % p.get_im_w();

	std::vector<active_edge_t> parent_A_chromosome = p.get_edge_candidates(parent_A);
	std::vector<active_edge_t> parent_B_chromosome = p.get_edge_candidates(parent_B);
	std::vector<active_edge_t> offspring_chromosome = parent_A_chromosome;

	for (int i = 0; i < parent_A_chromosome.size(); i++) {

	}

	return offspring_chromosome;
}

int  mutation_split_segments(Population &p, int ind_index) {
	std::vector<active_edge_t> chromosome = p.get_edge_candidates(ind_index);
	int loci = rand() % chromosome.size();

	int count = 10;
	while (chromosome[loci].active == 0 && count > 0) {
		loci = rand() % chromosome.size();
		count--;
	}
	if (chromosome[loci].active == 1) {
		p.split_segment(ind_index, loci, chromosome[loci].edge);
		return 1;
	}
	else {
		return 0;
	}
}

int  mutation_merge_segments(Population &p, int ind_index) {
	std::vector<active_edge_t> chromosome = p.get_edge_candidates(ind_index);
	int loci = rand() % chromosome.size();

	int count = 10;
	while (chromosome[loci].active == 1 && count > 0) {
		loci = rand() % chromosome.size();
		count--;
	}
	if (chromosome[loci].active == 0) {
		p.merge_segments(ind_index, loci, chromosome[loci].edge);
		return 1;
	}
	else {
		return 0;
	}
}
