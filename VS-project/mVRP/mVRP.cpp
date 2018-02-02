#include <iostream>
#include <set>
#include "mVRP.h"
#include "population.h"

bool operator<(const depot &right, const depot &left) {
	return (right.x + right.y < left.x + left.y);
}

bool operator<(const customer &right, const customer &left) {
	return (right.index < left.index);
}

void GA_mVRP() {
	std::cout << " Welcome to foor loop !" << std::endl;
	// initalize population
	std::set<customer> costumers;
	std::set<depot> depots;
	int n_vehicles = 0;
	int n_customers = 0;
	int n_depots = 0;
	int n_individuals = 0;

	// get data from function
	Population population(n_vehicles, n_customers, n_depots, n_individuals, costumers, depots);
}