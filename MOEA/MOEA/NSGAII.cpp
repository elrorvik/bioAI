#include"NSGAII.h"
#include"population.h"
#include"fitness.h"
#include"global.h"
#include<utility>
#include<algorithm>

std::vector<int> NSGAII(Population &p, const std::vector<pos> * entry_s) {
	
	int rank[N_IND]{}; // TODO: Make sure N_IND is the number of parents + children in this case
	double fitness_1[N_IND]{};
	double fitness_2[N_IND]{};

	// Non dominated sorting
	for (int ind_index = 0; ind_index < N_IND; ind_index++) { // TODO: Make sure N_IND is the number of parents + children in this case
		fitness_1[ind_index] = overall_deviation_ind(p, ind_index, entry_s[ind_index]);
		fitness_2[ind_index] = edge_value_ind(p, ind_index, entry_s[ind_index]);
	}

	for (int ind_index = 0; ind_index < N_IND; ind_index++) {
		for (int other_index = 0; other_index < N_IND; other_index++) {
			if (fitness_1[ind_index] > fitness_1[other_index] && fitness_2[ind_index] > fitness_2[other_index]) rank[ind_index]++;
		}
	}
	std::vector<int> temp;
	return temp;
}