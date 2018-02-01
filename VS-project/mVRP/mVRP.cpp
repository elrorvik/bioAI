#include <iostream>
#include <set>
#include "mVRP.h"
#include "population.h"

void GA_mVRP() {
	std::cout << " Welcome to foor loop !" << std::endl;
	// initalize population
	std::set<customer> costumers;
	std::set<depot> depots;
	int n_vehicles;
	int n_customers;
	int n_depots;
	int n_individuals;

	// get data from function
	Population population(n_vehicles, n_customers, n_depots, n_individuals, costumers, depots);



}