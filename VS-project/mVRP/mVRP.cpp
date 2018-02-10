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

void GA_mVRP() {
	std::string filename = "..\\..\\testing_data\\data_files\\p01";
	std::set<customer> customers;
	std::set<depot> depots;
	int n_vehicles;
	int n_customers;
	int n_depots;
	int n_individuals = 5;
	int n_parents = 2;


	read_data(filename, customers, depots, n_vehicles, n_customers, n_depots);


	// psedocode for whole algorithm:

	Population population(n_vehicles, n_customers, n_depots, n_individuals, n_parents, customers, depots);
	population.initialize_population_random();
	population.fitness_population_initalization();

	int improvement_in_fitness = 30;
	double best_fitness = 0.0;
	int generation = 0;
	int n_generations = 40;


	while (improvement_in_fitness > 0 && generation < n_generations) {
		//std::set<int> parent_index;
		//int n_crossover = 10; // of 100
		//population.selection_SUS(n_crossover, parent_index, population_selection);
		// apply crossover on parent_index
		//std::cout << "here 1" << std::endl;
		//std::cout << "start" << std::endl;

		int n_mutation = 4; // of 100
		std::set<int> parent_index;
		population.selection_SUS(n_mutation, parent_index, population_selection);
		population.insert_recombination_in_population(&parent_index);
		//std::cout << "here 2" << std::endl;

		int n_ellitisme = 3;
		int n_sus = n_individuals - n_ellitisme;

		std::set<int> selected_index;
		population.selection_ellitisme(n_ellitisme, selected_index, offspring_selection);
		//std::cout << "here 2b" << std::endl;
		population.selection_SUS(n_sus, selected_index, offspring_selection);

		/*std::cout << "\n Selected population " << std::endl;
		for (auto it = selected_index.begin(); it != selected_index.end(); it++) {
		std::cout << "Index :" << *it << " ,Fitness: " << population.get_fitness_individual(*it) << std::endl;
		}*/

		//std::cout << "here 3" << std::endl;
		population.remove_not_selected_members(&selected_index);
		double current_best_fitness = population.get_best_fitness();
		//std::cout << "here 4" << std::endl;
		if (current_best_fitness == best_fitness) {
			improvement_in_fitness--;
		}
		else if (best_fitness < current_best_fitness) {
			best_fitness = current_best_fitness;
			improvement_in_fitness = 10;
		}
		std::cout << std::endl << "Nr. generations: " << generation << ", best fitness: " << best_fitness << std::endl << std::endl;
		//population.print_population();
		generation++;

	}
	//population.print_population();

}