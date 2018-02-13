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
	std::string filename = "..\\..\\testing_data\\data_files\\p03";
	std::set<customer> customers;
	std::set<depot> depots;
	int n_vehicles;
	int n_customers;
	int n_depots;
	int n_individuals = 100;
	double parent_percentage = 0.02;
	double survivor_elitism_percentage = 1;
	double parent_elitism_percentage = 1;
	double volatile_percentage = 0.05;
	int n_parents = static_cast<int>(parent_percentage*n_individuals);
	int n_parents_elitism = static_cast<int>(parent_elitism_percentage*n_parents);
	int n_offspring = static_cast<int>(n_parents / 2);
	int n_elitism = static_cast<int>(survivor_elitism_percentage*n_individuals);
	int n_sus = n_individuals - n_elitism;

	read_data(filename, customers, depots, n_vehicles, n_customers, n_depots);


	// psedocode for whole algorithm:

	Population population(n_vehicles, n_customers, n_depots, n_individuals, n_parents, customers, depots);
	population.initialize_population_k_mean();
	population.fitness_population_initalization();

	int improvement_in_fitness = 2000;
	double best_fitness = DBL_MAX;
	int generation = 0;
	int n_generations = 2000;
	double second_generation_best_fitness = 0;
	int n_generations_without_improvement = 0;
	
	while (improvement_in_fitness > 0 && generation < n_generations) {
		// apply recombination on parent_index
		std::set<int> parent_index_1;
		population.selection_ellitisme(n_parents_elitism, parent_index_1, parent_selection);
		if (n_parents < n_individuals) population.selection_SUS(n_parents-n_parents_elitism, parent_index_1, parent_selection);

		population.insert_greedy_recombination_in_population_random_pairing(&parent_index_1);

		/*std::set<int> parent_index_2;
		int n_elitism_2 = 0;
		if (rand() % 2 == 0) n_elitism_2 = 1;
		if (n_parents < n_individuals) population.selection_SUS(n_parents_greedier, parent_index_2, parent_selection);
		population.selection_ellitisme(n_elitism_2, parent_index_2, parent_selection);

		population.insert_greedier_recombination_in_population_random_pairing(&parent_index_2);*/


		// apply mutation on offspring
		int era = 100;
		double swap_between_vehicle_percentage = 0;
		double inverse_internally_vehicle_percentage = 0.3;
		double customer_best_globally_percentage = 0.02*static_cast<int>(generation / (era/2));
		if (n_generations_without_improvement >= 4*era) {
			//swap_between_vehicle_percentage = 0;
			inverse_internally_vehicle_percentage = 0.2;
		}
		else if (n_generations_without_improvement >= era) {
			int n_eras_without_improvement = static_cast<int>(n_generations_without_improvement / era);
			//swap_between_vehicle_percentage += 0*n_eras_without_improvement;
			inverse_internally_vehicle_percentage -= 0.02*n_eras_without_improvement;
		}

		population.insert_mutation_in_offspring(0.1, swap_between_vehicle_percentage, inverse_internally_vehicle_percentage, customer_best_globally_percentage);
		/*int n_volatile_mutate = 0;
		if (static_cast<int>(n_offspring*0.05) > 0) {
			n_volatile_mutate = (rand() % static_cast<int>(n_offspring*0.05)) + static_cast<int>(n_offspring*0.05);
		}
		population.insert_volatile_mutation_in_offspring(n_volatile_mutate, 3);
		population.insert_volatile_mutation_in_offspring(3, 100);*/
		
		// select survivors
		std::set<int> selected_index;
		population.selection_ellitisme(n_elitism, selected_index, population_selection);
		population.selection_SUS(n_sus, selected_index, population_selection);

		/*std::cout << "\n Selected population " << std::endl;
		for (auto it = selected_index.begin(); it != selected_index.end(); it++) {
		std::cout << "Index :" << *it << " ,Fitness: " << population.get_fitness_individual(*it) << std::endl;
		}*/

		// remove non-survivors
		population.remove_not_selected_members(&selected_index);
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
		std::cout << "Nr. generation: " << generation << ", best fitness: " << best_fitness << std::endl;
		generation++;
	}

	population.write_result_to_file("..\\..\\solution.txt");
	std::cout << std::endl << std::endl << "Second generation best fitness: " << second_generation_best_fitness << ", best fitness at termination: " << best_fitness << std::endl;
	//population.print_population();

}