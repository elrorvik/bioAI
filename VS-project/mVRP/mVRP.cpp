#include <iostream>
#include <set>
#include "mVRP.h"
#include "population.h"
#include "file.h"

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
	string filename = "..\\..\\testing_data\\data_files\\p01";
	std::set<customer> customers;
	std::set<depot> depots;
	int n_vehicles;
	int n_customers;
	int n_depots;
	int n_individuals = 10;
	int n_parents = 2;


	read_data(filename, customers, depots, n_vehicles, n_customers, n_depots);


	Population population(n_vehicles, n_customers, n_depots, n_individuals,n_parents, customers, depots);
	population.initialize_population_random();
	population.fitness_population_initalization();
	population.print_population();


	// selection test
	int n_ellitisme = 2;
	int n_sus_candiates = 3;
	std::set<int> survival_index;
	population.selection_ellitisme(n_ellitisme,survival_index);
	population.selection_SUS(n_sus_candiates, survival_index);

	std::cout << "\n Selected population " << std::endl;
	for (auto it = survival_index.begin(); it != survival_index.end(); it++) {
		std::cout << "Index :" << *it <<" ,Fitness: " << population.get_fitness_individual(*it) << std::endl;
	}
	//population.test();

	// psedocode for whole algorithm:
	/*
	Population population(n_vehicles, n_customers, n_depots, n_individuals, n_parents, customers, depots);
	population.initialize_population_random();
	population.fitness_population_initalization();
	
	while (not termination condtion) {
		std::set<int> parent_index;
		int n_crossover = 10; // of 100
		population.selection_SUS(n_crossover, parent_index);

		// apply crossover on parent_index
		int n_mutation = 20; // of 100
		std::set<int> parent_index;
		population.selection_SUS(n_mutation, parent_index);

		// update fitness table, don't know how yet, but may use, if not done in mutation and cross_over already:
		population.fitness_population_initalization();

		int n_select_sus = 20;
		int n_select_ellitism = 10;
		population.selection_SUS(n_crossover, parent_index);
		population.selection_SUS(n_crossover, parent_index);

	}

	*/

	
}