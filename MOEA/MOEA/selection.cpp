#include"selection.h"
#include"population.h"
#include"global.h"
#include"fitness.h"
#include<utility>
#include<algorithm>
#include<cmath>
#include<set>
#include<iostream>

struct descending_comparator {
	bool operator() (const std::pair<double, int> p1, const std::pair<double, int> p2) {
		return p1.first < p2.first;
	}
	bool operator() (const std::pair<int, int> p1, const std::pair<int, int> p2) {
		return p1.first < p2.first;
	}
};

std::vector<int> NSGAII(Population &p, const std::vector<pos> * entry_s, int n_pop, std::vector<std::pair<int, int>> rank_individuals, const std::vector<std::pair<double, int>> &fitness_1, const std::vector<std::pair<double, int>> fitness_2) {

	double f_1_max;
	double f_2_max;
	double f_1_min;
	double f_2_min;
	MOEA_fitness_range(n_pop, fitness_1, fitness_2, f_1_max, f_2_max, f_1_min, f_2_min);

	descending_comparator smallest_value_comparator;
	std::sort(rank_individuals.begin(), rank_individuals.end(), smallest_value_comparator);


	// Elitism with crowding-distance-assigment for settling tournaments among individuals with the same rank
	int rank = 0;
	std::vector<int> selected_population;
	int ind_index = 0;
	while (selected_population.size() < N_IND) {
		std::vector<int> up_for_selection;
		
		for (ind_index; ind_index < n_pop; ind_index++) {
			if (rank_individuals[ind_index].first == rank) up_for_selection.push_back(rank_individuals[ind_index].second);
			else if (rank_individuals[ind_index].first > rank) break;
		}
		rank++;
		int tournament_size = up_for_selection.size();

		int n_vacancies = N_IND - selected_population.size();
		if (n_vacancies >= tournament_size) {
			selected_population.insert(selected_population.end(), up_for_selection.begin(), up_for_selection.end());
		}
		else {

			// Crowding distance
			std::vector<std::pair<double, int>> fitness_1_of_tournament;
			std::vector<std::pair<double, int>> fitness_2_of_tournament;
			fitness_1_of_tournament.reserve(tournament_size);
			fitness_2_of_tournament.reserve(tournament_size);
			for (int i = 0; i < tournament_size; i++) {
				fitness_1_of_tournament.push_back(fitness_1[up_for_selection[i]]);
				fitness_2_of_tournament.push_back(fitness_2[up_for_selection[i]]);
			}

			std::sort(fitness_1_of_tournament.rbegin(), fitness_1_of_tournament.rend(), smallest_value_comparator); // Crowding distance requires descending order
			std::sort(fitness_2_of_tournament.rbegin(), fitness_2_of_tournament.rend(), smallest_value_comparator);

			std::vector<std::pair<double, int>> crowding_distance;
			for (int i = 0; i < tournament_size; i++) {
				crowding_distance.push_back(std::make_pair(0, 0));
			}

			for (int i = 1, j = tournament_size - 2; i < tournament_size - 1; i++, j--) {
				// Following indexation should be correct because if they are not in eachothers reverse order, then the individuals would not have same rank
				crowding_distance[i].first = (fitness_1_of_tournament[i - 1].first - fitness_1_of_tournament[i + 1].first) / (f_1_max - f_1_min);
				crowding_distance[i].first += (fitness_2_of_tournament[j - 1].first - fitness_2_of_tournament[j + 1].first) / (f_2_max - f_2_min);
				crowding_distance[i].second = fitness_1_of_tournament[i].second;
			}
			crowding_distance[0].first = DBL_MAX;
			crowding_distance[0].second = fitness_1_of_tournament[0].second;
			crowding_distance[tournament_size - 1].first = DBL_MAX;
			crowding_distance[tournament_size - 1].second = fitness_1_of_tournament[tournament_size - 1].second;

			std::sort(crowding_distance.rbegin(), crowding_distance.rend(), smallest_value_comparator); // Sort in descending order using reverse iterator

			// Add individuals with the highest crowding distance first until population full
			for (int i = 0; i < n_vacancies; i++) {
				selected_population.push_back(crowding_distance[i].second);
			}
		}
	}
	return selected_population;
}

void MOEA_fitness(Population &p, int n_pop,const std::vector<pos> * entry_s, std::vector<std::pair<double, int>> &fitness_1, std::vector<std::pair<double, int>> &fitness_2) {

	descending_comparator smallest_value_comparator;

	// Non dominated sorting, and store max and min values for each objective functions
	for (int ind_index = 0; ind_index < n_pop; ind_index++) {
		fitness_1[ind_index] = std::make_pair(overall_deviation_ind(p, ind_index, entry_s[ind_index]), ind_index);
		fitness_2[ind_index] = std::make_pair(edge_value_ind(p, ind_index, entry_s[ind_index]), ind_index);
	}
}

void MOEA_rank(int n_pop, std::vector<std::pair<int, int>> &rank_individuals, const std::vector<std::pair<double, int>> &fitness_1, const std::vector<std::pair<double, int>> &fitness_2) {
	for (int ind_index = 0; ind_index < n_pop; ind_index++) {
		rank_individuals[ind_index].first = 0;
		for (int other_index = 0; other_index < n_pop; other_index++) {
			if (fitness_1[ind_index].first > fitness_1[other_index].first && fitness_2[ind_index].first > fitness_2[other_index].first) rank_individuals[ind_index].first++;
		}
	}
}

void MOEA_fitness_range(int n_pop, const std::vector<std::pair<double, int>> &fitness_1, const std::vector<std::pair<double, int>> &fitness_2, double &f_1_max, double &f_2_max, double &f_1_min, double &f_2_min) {
	f_1_max = -DBL_MAX;
	f_2_max = -DBL_MAX;
	f_1_min = DBL_MAX;
	f_2_min = DBL_MAX;
	for (int ind_index = 0; ind_index < n_pop; ind_index++) {
		if (fitness_1[ind_index].first > f_1_max) f_1_max = fitness_1[ind_index].first;
		if (fitness_2[ind_index].first > f_2_max) f_2_max = fitness_2[ind_index].first;
		if (fitness_1[ind_index].first < f_1_min) f_1_min = fitness_1[ind_index].first;
		if (fitness_2[ind_index].first < f_2_min) f_2_min = fitness_2[ind_index].first;
	}
}

std::vector<int> rank_tournament_selection(Population &p, const std::vector<pos> * entry_s, int n_pop, int tournament_size, int num_chosen, const std::vector<std::pair<int, int>> &rank_individuals) {

	const double CHANCE = 0.3;
	std::vector<int> selected_tournament;
	while (selected_tournament.size() < tournament_size) {
		int ind_index = rand() % n_pop;
		if (std::find(selected_tournament.begin(), selected_tournament.end(), ind_index) == selected_tournament.end()) selected_tournament.push_back(ind_index);
	}

	std::vector<std::pair<int, int>> rank_of_tournament;
	for (int i = 0; i < tournament_size; i++) {
		rank_of_tournament.push_back(rank_individuals[i]);
	}
	descending_comparator smallest_value_comparator;
	std::sort(rank_of_tournament.begin(), rank_of_tournament.end(), smallest_value_comparator);

	// Perform tournament
	std::vector<int> selected_individuals;
	selected_individuals.reserve(num_chosen);
	for (int i = 0; i < tournament_size; i++) {
		if (num_chosen - selected_individuals.size() <= tournament_size - i) {
			selected_individuals.push_back(rank_of_tournament[i].second);
		}
		else {
			double outcome = (rand() % 1000) / 1000.0;
			if (outcome < CHANCE*pow(1 - CHANCE, i)) selected_individuals.push_back(rank_of_tournament[i].second);
		}
	}
	return selected_individuals;
}

std::vector<int> fitness_tournament_selection(Population &p, const std::vector<pos> * entry_s, int n_pop, int tournament_size, int num_chosen, double weight_1, double weight_2) {

	const double CHANCE = 0.3;

	std::vector<std::pair<double, int>> fitness;
	fitness.reserve(2 * N_IND);

	// Non dominated sorting, and store max and min values for each objective functions
	for (int ind_index = 0; ind_index < n_pop; ind_index++) {
		fitness.push_back(std::make_pair(overall_deviation_ind(p, ind_index, entry_s[ind_index])*weight_1 + edge_value_ind(p, ind_index, entry_s[ind_index])*weight_2, ind_index));
	}
	descending_comparator smallest_value_comparator;
	std::sort(fitness.begin(), fitness.end(), smallest_value_comparator);
	
	// Perform tournament
	std::vector<int> selected_individuals;
	selected_individuals.reserve(num_chosen);
	for (int i = 0; i < tournament_size; i++) {
		if (num_chosen - selected_individuals.size() <= tournament_size - i) {
			selected_individuals.push_back(fitness[i].second);
		}
		else {
			double outcome = (rand() % 1000) / 1000.0;
			if (outcome < CHANCE*pow(1 - CHANCE, i)) selected_individuals.push_back(fitness[i].second);
		}
	}
	return selected_individuals;
}