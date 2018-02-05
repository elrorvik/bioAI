#pragma once
#include "mVRP.h"
#include <vector>
#include <set>


class Population
{
private:
	std::vector<int> ** population;
	int n_vehicles;
	int n_customers;
	int n_depots;
	int n_individuals;
	customer * customers;
	depot * depots;
public:
	//Population(int n_vehicles, int n_customers, int n_depots, int n_individuals,std::set<customer> &customers, std::set<depot> &depots);
	//Population(int n_vehicles, int n_customers, int n_depots, int n_individuals, std::set<customer> &customers, std::set<depot> &depots);
	Population(int n_vehicles, int n_customers, int n_depots, int n_individuals, std::set<customer> &customers, std::set<depot> &depots);
	void initialize_population_random();
	~Population();
	customer get_customer(int index);
	void print_population();
	int get_depot_vehicle_index(int vehicle_index, int depot_index);
	customer get_customer(int individual_index, int depot_index, int vehicle_index, int customer_index_in_queue);
	int get_customer_index(int individual_index, int depot_index, int vehicle_index, int customer_index_in_queue);
	void print_vehicles_customer_queue(int individual_index, int vehicle_index, int depot_index);
	int get_vehicle_queue_size(int individual_index, int depot_index, int vehicle_index);

	double fitness_individual(std::vector<int> *individual);
	void mutate_swap_internally_vehicle(std::vector<int> *individual);
	void mutate_insert_between_vehicle(std::vector<int> *individual);

	void test();

};

