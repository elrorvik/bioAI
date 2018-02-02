#pragma once
#include "mVRP.h"
#include <set>


class Population
{
private:
	int ** population;
	int n_vehicles;
	int n_customers;
	int n_depots;
	int n_individuals;
	customer * customers;
	depot * depots;
public:
	Population(int n_vehicles, int n_customers, int n_depots, int n_individuals,std::set<customer> &customers, std::set<depot> &depots);
	void initialize_population_random();
	~Population();
	customer get_customer(int index);
	customer get_customer(int individual_index, int vehicle_index, int customer_index_in_queue);
	void print_vehicles_customer_queue(int individual_index, int vehicle_index);
	void print_population();

};

