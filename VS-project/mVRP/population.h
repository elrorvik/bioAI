#pragma once
#include "mVRP.h"
#include <set>


class Population
{
private:
	customer * *population;
	int n_vehicles;
	int n_customers;
	int n_depots;
	int n_individuals;
public:
	Population(int n_vehicles, int n_customers, int n_depots, int n_individuals, std::set<customer> &costumers, std::set<depot> &depots);
	~Population();



};

