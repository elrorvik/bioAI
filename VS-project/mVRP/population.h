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
	std::vector<int> * customers_available_by_depot;
	int n_vehicles;
	int n_customers;
	int n_depots;
	int n_individuals;
	int n_parents;
	int n_offspring;
	customer* customers;
	depot* depots;
	double best_fitness;
	int best_fitness_index;

	std::vector<int> vehicle_A_customer_buffer;
	std::vector<int> vehicle_B_customer_buffer;
public:
	void remove_not_selected_members(std::set<int>* selected_index);
	void fitness_individual_initalization(std::vector<int> *individual, int index_individual);
	void insert_mutation_in_offspring(double insert_in_vehicle_percentage, double swap_between_vehicle_percentage, double inverse_internally_vehicle_percentage, double customer_best_globally_percentage);
	void insert_volatile_mutation_in_offspring(int n_mutate, int volatility);
	void insert_greedy_recombination_in_population_random_pairing(std::set<int>* parent_index);
	void insert_greedier_recombination_in_population_random_pairing(std::set<int>* parent_index);
	void insert_recombination_in_population_deterministic_pairing(std::set<int>* parent_index);
	Population(int n_vehicles, int n_customers, int n_depots, int n_individuals, int n_ellitisme, std::set<customer> &customers, std::set<depot> &depots);
	void initialize_population_random();
	void initialize_population_k_mean();
	void initialize_depot_customer_availability();
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
	void set_fitness_vehicle(int individual_index, int vehicle_index);
	customer get_customer(int individual_index, int depot_index, int vehicle_index, int customer_index_in_queue);
	int get_customer_index(int individual_index, int depot_index, int vehicle_index, int customer_index_in_queue);
	void print_vehicles_customer_queue(int individual_index, int vehicle_index, int depot_index);
	int get_vehicle_queue_size(int individual_index, int depot_index, int vehicle_index);

	void fitness_population_initalization();
	void mutate_swap_internally_vehicle(std::vector<int> *individual, int index);
	void mutate_swap_between_vehicle(std::vector<int> *individual, int index);
	void mutate_swap_between_depot(std::vector<int> *individual, int index);
	//void mutate_optimize_vehicle(std::vector<int> *individual, int individual_index, int vehicle_index);
	void mutate_inverse_internally_vehicle(std::vector<int> *individual, int individual_index);
	void mutate_trim_bloated_vehicle(std::vector<int> *individual, int individual_index);
	void mutate_customer_best_globally(std::vector<int> *individual, int individ_index);
	void mutate_route_segment_best_globally(std::vector<int> *individual, int individ_index);
	void recombination_BCRC_greedier(std::vector<int> *parent_A, std::vector<int> *parent_B);
	void recombination_BCRC_greedy(std::vector<int> *parent_A, std::vector<int> *parent_B);

	void set_n_offspring(int num);
	void selection_SUS(int n_pointers, std::set<int>& survival_index, selection_on selection_type);
	void selection_ellitisme(int n_ellitism, std::set<int>& survival_index, selection_on selection_type);

	void write_result_to_file(std::string filename);
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

int random_number(int modula);

struct pot_customer_loci {
	int vehicle_i;
	int depot_i;
	int it_diff;
	double fitness;

	pot_customer_loci(int vehicle_i, int depot_i, int it_diff, double fitness) : vehicle_i(vehicle_i), depot_i(depot_i), it_diff(it_diff), fitness(fitness) {};
	pot_customer_loci() : vehicle_i(0), depot_i(0), it_diff(0), fitness(0) {};
};

bool operator<(const pot_customer_loci &right, const pot_customer_loci &left);


double distance(const customer &c, const depot &d);
double distance(const customer &c1, const customer &c2);
double distance(const depot &d1, const customer &d2);