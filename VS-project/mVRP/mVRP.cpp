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
	int n_individuals = 100;
	double parent_percentage = 1;
	double survivor_elitism_percentage = 0.1;
	int n_parents = static_cast<int>(parent_percentage*n_individuals);
	int n_elitism = static_cast<int>(survivor_elitism_percentage*n_individuals);
	int n_sus = n_individuals - n_elitism;


	read_data(filename, customers, depots, n_vehicles, n_customers, n_depots);


	// psedocode for whole algorithm:

	Population population(n_vehicles, n_customers, n_depots, n_individuals, n_parents, customers, depots);
	population.initialize_population_random();
	population.fitness_population_initalization();

	int improvement_in_fitness = 30;
	double best_fitness = 0.0;
	int generation = 0;
	int n_generations = 40;
	double second_generation_best_fitness = 0;

	while (improvement_in_fitness > 0 && generation < n_generations) {
		// apply recombination on parent_index
		std::set<int> parent_index;
		population.selection_SUS(n_parents, parent_index, population_selection);
		std::cout << "Selection finished" << std::endl;
		population.insert_recombination_in_population(&parent_index);

		std::cout << "Recombination finished" << std::endl;

		// apply mutation on offspring

		// ...

		// select survivors
		std::set<int> selected_index;
		std::cout << n_elitism << std::endl;
		population.selection_ellitisme(n_elitism, selected_index, offspring_selection);
		std::cout << "Number selected by elitism: " << selected_index.size() << std::endl;
		population.selection_SUS(n_sus, selected_index, offspring_selection);

		/*std::cout << "\n Selected population " << std::endl;
		for (auto it = selected_index.begin(); it != selected_index.end(); it++) {
		std::cout << "Index :" << *it << " ,Fitness: " << population.get_fitness_individual(*it) << std::endl;
		}*/

		// remove non-survivors
		population.remove_not_selected_members(&selected_index);
		double current_best_fitness = population.get_best_fitness();
		if (current_best_fitness == best_fitness) {
			improvement_in_fitness--;
		}
		else if (best_fitness < current_best_fitness) {
			best_fitness = current_best_fitness;
			improvement_in_fitness = 10;
		}
		if (generation == 0) {
			second_generation_best_fitness = best_fitness;
		}
		std::cout << std::endl << "Nr. generation: " << generation << ", best fitness: " << best_fitness << std::endl << std::endl;
		generation++;

	}

	std::cout << std::endl << std::endl << "Second generation best fitness: " << second_generation_best_fitness << ", best fitness at termination: " << best_fitness << std::endl;
	//population.print_population();

}