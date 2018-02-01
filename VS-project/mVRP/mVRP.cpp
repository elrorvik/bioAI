#include <iostream>
#include <set>
#include "mVRP.h"
#include "population_hadling.h"

void GA_mVRP() {
	std::cout << " Welcome to foor loop !" << std::endl;
	// initalize population
	std::set<customer> costumers;
	std::set<depot> depots;
	int n_vehicles;
	int n_customers;
	int n_depots;

	// get data from function
	customer** population;
	initialize_population(population,n_vehicles,n_customers,n_depots,costumers,depots);


}