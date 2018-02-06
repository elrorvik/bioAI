#pragma once
#include "mVRP.h"
#include <vector>
#include <set>

enum selection_on{offspring_selection, population_selection};

class Population
{
private:
	std::vector<int> ** population;
	std::vector<int> ** offspring;
	double ** fitness_vehicle;
	double *fitness_individual;
	int* index_high_rank_individuals;
	int * parent_index;
	int fitness_total;
	int n_vehicles;
	int n_customers;
	int n_depots;
	int n_individuals;
	int n_parents;
	int n_ellitisme;
	customer* customers;
	depot* depots;
public:

	Population(int n_vehicles, int n_customers, int n_depots, int n_individuals, int n_ellitisme, std::set<customer> &customers, std::set<depot> &depots);
	void initialize_population_random();
	~Population();
	customer get_customer(int index);
	void print_population();
	int get_depot_vehicle_index(int vehicle_index, int depot_index);
	double get_fitness_individual(int individual_index);
	customer get_customer(int individual_index, int depot_index, int vehicle_index, int customer_index_in_queue);
	int get_customer_index(int individual_index, int depot_index, int vehicle_index, int customer_index_in_queue);
	void print_vehicles_customer_queue(int individual_index, int vehicle_index, int depot_index);
	int get_vehicle_queue_size(int individual_index, int depot_index, int vehicle_index);

	void fitness_population_initalization();
	void mutate_swap_internally_vehicle(std::vector<int> *individual);
	void mutate_insert_between_vehicle(std::vector<int> *individual);

	void selection_SUS(int n_pointers, std::set<int>& survival_index);
	void selection_ellitisme(int n_ellitism, std::set<int>& survival_index);
	void test();

};

