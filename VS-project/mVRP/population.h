#pragma once
#include "mVRP.h"
#include <vector>
#include <set>



class Population
{
private:
	std::vector<int> ** population;
	double ** fitness_vehicle;
	double *fitness_individual;
	int n_vehicles;
	int n_customers;
	int n_depots;
	int n_individuals;
	int n_parents;
	int n_offspring;
	customer* customers;
	depot* depots;
	double best_fitness;

	std::vector<int> vehicle_A_customer_buffer;
	std::vector<int> vehicle_B_customer_buffer;

public:
	void remove_not_selected_members(std::set<int>* selected_index);
	void fitness_individual_initalization(std::vector<int> *individual, int index_individual);
	void insert_mutation_in_offspring(int n_mutate);
	void insert_volatile_mutation_in_offspring(int n_mutate, int volatility);
	void insert_recombination_in_population_random_pairing(std::set<int>* parent_index);
	void insert_recombination_in_population_deterministic_pairing(std::set<int>* parent_index);
	Population(int n_vehicles, int n_customers, int n_depots, int n_individuals, int n_ellitisme, std::set<customer> &customers, std::set<depot> &depots);
	void initialize_population_random();
	~Population();
	customer get_customer(int index);
	void print_population();
	void print_individual(int individual_index);
	void validate_individual(int individual_index);
	int get_depot_vehicle_index(int vehicle_index, int depot_index);
	double get_best_fitness();
	double get_fitness_individual(int individual_index);
	double get_fitness_individual(std::vector<int> *individual);
	double get_fitness_vehicle(int depot_number, std::vector<int> vehicle);
	customer get_customer(int individual_index, int depot_index, int vehicle_index, int customer_index_in_queue);
	int get_customer_index(int individual_index, int depot_index, int vehicle_index, int customer_index_in_queue);
	void print_vehicles_customer_queue(int individual_index, int vehicle_index, int depot_index);
	int get_vehicle_queue_size(int individual_index, int depot_index, int vehicle_index);

	void fitness_population_initalization();
	void mutate_swap_internally_vehicle(std::vector<int> *individual, int index);
	void mutate_insert_between_vehicle(std::vector<int> *individual);
	void recombination_BCRC(std::vector<int> *parent_A, std::vector<int> *parent_B);

	void set_n_offspring(int num);
	void selection_SUS(int n_pointers, std::set<int>& survival_index, selection_on selection_type);
	void selection_ellitisme(int n_ellitism, std::set<int>& survival_index, selection_on selection_type);

};

struct wheel_piece_t {
	double l_b = 0;
	double u_b = 0;
	int individual_index = 0;
	wheel_piece_t(double l_b, double u_b, double individual_index) : l_b(l_b), u_b(u_b), individual_index(individual_index) {};
	wheel_piece_t() : l_b(0), u_b(0), individual_index(-1) {};
};

struct fitness_index {
	int index;
	double fitness;
	fitness_index(double index, double fitness) : index(index), fitness(fitness) {};
	fitness_index() : index(-1), fitness(0) {}
};

bool operator<(const fitness_index &right, const fitness_index &left);

