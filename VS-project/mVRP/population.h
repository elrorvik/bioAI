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
	void initialize_random();
	~Population();

};

