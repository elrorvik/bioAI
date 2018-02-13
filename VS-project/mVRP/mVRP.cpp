#include <iostream>
#include <set>
#include "mVRP.h"
#include "population.h"
#include "file.h"
#include <string>

/*void customer::operator=(const customer &other) {
	this->index = other.index;
	this->x = other.x;
	this->y = other.y;
	this->duration = other.duration;
	this->demand = other.demand;
}*/

bool operator<(const depot &right, const depot &left) {
	return (right.x + right.y < left.x + left.y);
}

bool operator<(const customer &right, const customer &left) {
	return (right.index < left.index);
}

void GA_mVRP(int n_individuals, double parent_percentage, double survivor_elitism_percentage, double parent_elitism_percentage, std::string filename) {
	std::set<customer> customers;
	std::set<depot> depots;
	int n_vehicles;
	int n_customers;
	int n_depots;
	int n_parents = static_cast<int>(parent_percentage*n_individuals);
	int n_parents_elitism = static_cast<int>(parent_elitism_percentage*n_parents);
	int n_parents_sus = n_parents - n_parents_elitism;
	int n_survivor_elitism = static_cast<int>(survivor_elitism_percentage*n_individuals);
	int n_survivor_sus = n_individuals - n_survivor_elitism;

	read_data(filename, customers, depots, n_vehicles, n_customers, n_depots);

	/*std::cout << "p01 optimal value: " << 576.87 << std::endl;
	std::cout << "5% target: " << 576.87 + 576.87*0.05 << std::endl;
	std::cout << "10% target: " << 576.87 + 576.87*0.1 << std::endl;

	std::cin.get();*/

	// psedocode for whole algorithm:

	Population population(n_vehicles, n_customers, n_depots, n_individuals, n_parents, customers, depots);
	population.initialize_population_k_mean();
	population.fitness_population_initalization();
	population.initialize_depot_customer_availability();

	int improvement_in_fitness = 2000;
	double best_fitness = DBL_MAX;
	int generation = 0;
	int n_generations = 2000;
	double second_generation_best_fitness = 0;
	int n_generations_without_improvement = 0;
	
	while (improvement_in_fitness > 0 && generation < n_generations) {
		// apply recombination on parent_index
		std::set<int> parent_index;
		population.selection_ellitisme(n_parents_elitism, parent_index, parent_selection);
		population.selection_SUS(n_parents_sus, parent_index, parent_selection);
		population.insert_greedy_recombination_in_population_random_pairing(&parent_index);

		/*std::set<int> parent_index_2;
		int n_elitism_2 = 0;
		if (rand() % 2 == 0) n_elitism_2 = 1;
		if (n_parents < n_individuals) population.selection_SUS(n_parents_greedier, parent_index_2, parent_selection);
		population.selection_ellitisme(n_elitism_2, parent_index_2, parent_selection);

		population.insert_greedier_recombination_in_population_random_pairing(&parent_index_2);*/


		// apply mutation on offspring by percentages
		int era = 10;
		double intra_percentage = 0.25;
		double inter_percentage = 0.25;
		double inverse_intra_vehicle_perc = 0.33*intra_percentage;
		double swap_intra_depot_perc = 0.33*intra_percentage;
		double customer_intra_depot_optimally_perc = 0.33*intra_percentage;
		double insert_inter_depot_perc = 1 * inter_percentage;
		double include_neighbour_perc = 0.2;

		if (generation % era != 0) population.insert_intra_mutation_in_offspring(inverse_intra_vehicle_perc, swap_intra_depot_perc, customer_intra_depot_optimally_perc);
		else population.insert_inter_mutation_in_offspring(insert_inter_depot_perc, include_neighbour_perc);

		// select survivors
		std::set<int> selected_index;
		population.selection_ellitisme(n_survivor_elitism, selected_index, population_selection);
		population.selection_SUS(n_survivor_sus, selected_index, population_selection);

		// remove non-survivors
		population.remove_not_selected_members(&selected_index);

		// check fitness progression
		double current_best_fitness = population.get_best_fitness();
		if (current_best_fitness >= best_fitness) {
			improvement_in_fitness--;
			n_generations_without_improvement++;
		}
		else {
			best_fitness = current_best_fitness;
			improvement_in_fitness = 4000;
			n_generations_without_improvement = 0;
		}
		if (generation == 0) {
			second_generation_best_fitness = best_fitness;
		}

		// give update, and iterate
		std::cout << "Nr. generation: " << generation << ", best fitness: " << best_fitness << std::endl;
		generation++;
	}

	population.write_result_to_file("..\\..\\solution.txt");
	std::cout << std::endl << std::endl << "Second generation best fitness: " << second_generation_best_fitness << ", best fitness at termination: " << best_fitness << std::endl;
	//population.print_population();

}