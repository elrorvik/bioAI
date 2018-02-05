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

	read_data(filename, customers, depots, n_vehicles, n_customers, n_depots);


	Population population(n_vehicles, n_customers, n_depots, n_individuals, customers, depots);
	population.initialize_population_random();
	population.print_population();
	population.test();




	
}