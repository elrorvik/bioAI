#include <iostream>
#include <iomanip>
#include "population.h"
#include "mVRP.h"
#include <time.h> 
#include <vector>
#include <set>
#include <utility>
#include <cmath>
#include <algorithm>


Population::Population(int n_vehicles, int n_customers, int n_depots, int n_individuals, int n_ellitisme, std::set<customer> &customers, std::set<depot> &depots) {
	this->population = new std::vector<int>*[n_individuals * 2];
	//this->offspring = new std::vector<int>*[n_individuals];
	this->fitness_vehicle = new double*[n_individuals * 2];
	this->fitness_individual = new double[n_individuals * 2];
	this->n_vehicles = n_vehicles;
	this->n_customers = n_customers;
	this->n_depots = n_depots;
	this->n_individuals = n_individuals;
	this->best_fitness = 0;
	this->n_offspring = 0;

	std::cout << "customers: " << this->n_customers << ", depots: " << this->n_depots << " individuals: " << this->n_individuals << " vehicles: " << this->n_vehicles << std::endl;

	for (int i = 0; i < n_individuals * 2; i++)
	{
		this->population[i] = new std::vector<int>[n_vehicles * n_depots];
		this->fitness_vehicle[i] = new double[n_vehicles*n_depots];
	}

	int  index = 0;
	this->customers = new customer[n_customers];

	for (auto it = customers.begin(); it != customers.end(); it++) {
		this->customers[index] = *it;
		index++;
	}

	index = 0;
	this->depots = new depot[n_customers];
	for (auto it = depots.begin(); it != depots.end(); it++) {
		this->depots[index] = *it;
		index++;
	}
	/*
	std::cout << "Customers" << std::endl;
	for (int i = 0; i < n_customers; i++) {
	std::cout << this->customers[i].index << " " << this->customers[i].x << " " << this->customers[i].y << " " << this->customers[i].duration << " " << this->customers[i].demand << std::endl;
	}

	std::cout << "depots" << std::endl;
	for (int i = 0; i < n_depots; i++) {
	std::cout << this->depots[i].x << " " << this->depots[i].y << " " << this->depots[i].max_duration_per_vehicle << " " << this->depots[i].max_load_per_vehicle << std::endl;
	}*/


}

Population::~Population() {

	for (int i = 0; i < n_individuals; i++)
	{
		//std::cout << i << " ";
		//this->print_vehicles_customer_queue(i, 0, 2);
		delete[] this->population[i];
		delete[] this->fitness_vehicle[i];
	}
	delete[] this->population;
	delete[] this->fitness_vehicle;
}

void Population::initialize_population_random() {
	for (int individual_index = 0; individual_index < n_individuals; individual_index++) {
		std::vector<int> available_customer_indexes; // customers needed to be placed
													 //int sum = 0;
		for (int i = 0; i < n_customers; i++) available_customer_indexes.push_back(i);
		for (int i = 0; i < n_customers; i++) {
			int random_index = rand() % available_customer_indexes.size();
			int customer_index = available_customer_indexes[random_index];
			available_customer_indexes.erase(available_customer_indexes.begin() + random_index);
			int depot_random_index = rand() % n_depots;
			int vehicle_random_index = rand() % n_vehicles;
			int index_depot_vehicle = get_depot_vehicle_index(vehicle_random_index, depot_random_index);
			this->population[individual_index][index_depot_vehicle].push_back(customer_index);
			//sum += customer_index;
		}
		//std::cout << " initialize inidividual" << individual_index << "check sum : " << sum << std::endl;
		//std::cout << individual_index << " ";
		//this->print_vehicles_customer_queue(individual_index, 0, 2);
	}
}

void Population::print_population() {

	for (int individual_index = 0; individual_index < n_individuals; individual_index++) {
		std::cout << "*** Individual. " << individual_index << " Fitness score: "  <<fitness_individual[individual_index] << " ***"<<std::endl;
		for (int depot_index = 0; depot_index < n_depots; depot_index++) {
			std::cout << "Depot nr.: " <<  depot_index << std::endl;
			for (int vehicle_index = 0; vehicle_index < n_vehicles; vehicle_index++) {
				this->print_vehicles_customer_queue(individual_index, vehicle_index, depot_index);
			}
		}
	}
}

void Population::print_individual(int individual_index) {
	std::cout << "*** Individual. " << individual_index << " Fitness score: " << fitness_individual[individual_index] << " ***" << std::endl;
	for (int depot_index = 0; depot_index < n_depots; depot_index++) {
		std::cout << "Depot nr.: " << depot_index << std::endl;
		for (int vehicle_index = 0; vehicle_index < n_vehicles; vehicle_index++) {
			this->print_vehicles_customer_queue(individual_index, vehicle_index, depot_index);
		}
	}
}

void Population::validate_individual(int individual_index) {
	std::vector<int> customers_in_individual;
	for (int depot_index = 0; depot_index < n_depots; depot_index++) {
		for (int vehicle_index = 0; vehicle_index < n_vehicles; vehicle_index++) {
			for (int customer_index = 0; customer_index < population[individual_index][vehicle_index + depot_index * n_vehicles].size(); customer_index++) {
				customers_in_individual.push_back(population[individual_index][vehicle_index + depot_index * n_vehicles][customer_index]);
			}
		}
	}
	if (customers_in_individual.size() != n_customers) {
		std::cout << "n_customers: " << n_customers << ", number of customers in this individual: " << customers_in_individual.size() << std::endl;
		std::cout << "Invalid individual. The program will now exit." << std::endl;
		std::cin.get();
		exit(1);
	}
	sort(customers_in_individual.begin(), customers_in_individual.end());
	int last_customer = 0;
	for (auto it = customers_in_individual.begin(); it != customers_in_individual.end(); it++) {
		if ((it != customers_in_individual.begin()) && (last_customer + 1 != *it)) {
			std::cout << "Previous customer in individual: " << last_customer << ", next customer: " << *it << std::endl;
			std::cout << "Invalid individual. The program will now exit." << std::endl;
			std::cin.get();
			exit(1);
		}
		last_customer = *it;
	}
}

customer Population::get_customer(int index) {
	if (index >= n_customers || index < 0) {
		return customer();
	}
	return this->customers[index];
}
int Population::get_depot_vehicle_index(int vehicle_index, int depot_index) {
	return this->n_vehicles*depot_index + vehicle_index;
}

customer Population::get_customer(int individual_index, int depot_index, int vehicle_index, int customer_index_in_queue) {
	int customer_index = this->population[individual_index][get_depot_vehicle_index(vehicle_index, depot_index)][customer_index_in_queue];
	if (customer_index != invalid_city) {
		return this->get_customer(customer_index);
	}
	else {
		return customer();
	}

}

int Population::get_customer_index(int individual_index, int depot_index, int vehicle_index, int customer_index_in_queue) {
	int customer_index = this->population[individual_index][get_depot_vehicle_index(vehicle_index, depot_index)][customer_index_in_queue];
	return customer_index;

}

int Population::get_vehicle_queue_size(int individual_index, int depot_index, int vehicle_index) {
	return this->population[individual_index][get_depot_vehicle_index(vehicle_index, depot_index)].size();

}

double Population::get_fitness_individual(int individual_index) {
	return this->fitness_individual[individual_index];
}


double Population::get_fitness_individual(std::vector<int> *individual) {

	/*Fitness is "duration of service" = travel distance
	+ punishment of not satisfying all customers on a route
	+ punishment of a route being too long*/
	double duration = 0;
	double load = 0;
	double punishment = 0;
	double individual_fitness = 0;

	for (int i = 0; i < n_vehicles*n_depots; i++) {
		if (individual[i].size() == 0) {
			continue;
		}
		customer c = get_customer(individual[i][0]);
		customer cn = c;
		depot d = depots[static_cast<int>(std::floor(i / n_vehicles + 0.001 / n_vehicles))];
		//std::cout << static_cast<int>(std::floor(i / n_vehicles + 0.001 / n_vehicles)) << std::endl;

		duration += sqrt(pow(c.x - d.x, 2) + pow(c.y - d.y, 2));
		for (int j = 1; j < individual[i].size(); j++) {
			c = cn;
			cn = get_customer(individual[i][j]);
			duration += sqrt(pow(cn.x - c.x, 2) + pow(cn.y - c.y, 2));
			duration += c.duration;
			load += c.demand;
		}
		duration += sqrt(pow(cn.x - d.x, 2) + pow(cn.y - d.y, 2));
		load += cn.demand;

		if (duration > d.max_duration_per_vehicle && d.max_duration_per_vehicle > 0) punishment += 10 * (duration - d.max_duration_per_vehicle + 1) + 10;
		if (load > d.max_load_per_vehicle && d.max_load_per_vehicle > 0) punishment +=  10 * (load - d.max_load_per_vehicle + 1) + 10;

		double fitness = duration + punishment;
		individual_fitness += fitness;
	}
	return individual_fitness;
}

double Population::get_fitness_vehicle(int depot_number, std::vector<int> vehicle) {
	if (vehicle.size() == 0) return 0;
	
	double duration = 0;
	double load = 0;
	double fitness = 0;
	double punishment = 0;

	customer c = get_customer(vehicle[0]);
	customer cn = c;
	depot d = depots[depot_number];

	duration += sqrt(pow(c.x - d.x, 2) + pow(c.y - d.y, 2));
	for (int j = 1; j < vehicle.size(); j++) {
		c = cn;
		cn = get_customer(vehicle[j]);
		duration += sqrt(pow(cn.x - c.x, 2) + pow(cn.y - c.y, 2));
		duration += c.duration;
		load += c.demand;
	}
	duration += sqrt(pow(cn.x - d.x, 2) + pow(cn.y - d.y, 2));
	load += cn.demand;

	if (duration > d.max_duration_per_vehicle && d.max_duration_per_vehicle > 0) punishment += 10 * (duration - d.max_duration_per_vehicle + 1) + 10;
	if (load > d.max_load_per_vehicle && d.max_load_per_vehicle > 0) punishment += 10 * (load - d.max_load_per_vehicle + 1) + 10;
	fitness = duration + punishment;
	return fitness;
}

void Population::print_vehicles_customer_queue(int individual_index, int vehicle_index, int depot_index) {
	int sum = 0;
	std::cout << "Vehicle nr. " << vehicle_index << " ,Queue: ";
	for (int i = 0; i < get_vehicle_queue_size(individual_index, depot_index, vehicle_index); i++) {
		int customer_index = this->get_customer_index(individual_index, depot_index, vehicle_index, i);

		std::cout << customer_index << " ";
	}
	std::cout << std::endl;
}

void Population::set_n_offspring(int num) {
	this->n_offspring = num;
}


void Population::fitness_population_initalization() {

	for (int individual_index = 0; individual_index < n_individuals + n_offspring; individual_index++) {
		fitness_individual_initalization(population[individual_index], individual_index);
	}
}

void Population::fitness_individual_initalization(std::vector<int> *individual, int index_individual) {

	/*Fitness is "duration of service" = travel distance
	+ punishment of not satisfying all customers on a route
	+ punishment of a route being too long*/

	double duration = 0;
	double load = 0;
	double punishment = 0;
	double individual_fitness = 0;

	for (int i = 0; i < n_vehicles*n_depots; i++) {
		if (individual[i].size() == 0) {
			fitness_vehicle[index_individual][i] = 0;
			continue;
		}
		customer c = get_customer(individual[i][0]);
		customer cn = c;
		depot d = depots[static_cast<int>(std::floor(i / n_vehicles + 0.001 / n_vehicles))];
		//std::cout << static_cast<int>(std::floor(i / n_vehicles + 0.001 / n_vehicles)) << std::endl;

		duration += sqrt(pow(c.x - d.x, 2) + pow(c.y - d.y, 2));
		for (int j = 1; j < individual[i].size(); j++) {
			c = cn;
			cn = get_customer(individual[i][j]);
			duration += sqrt(pow(cn.x - c.x, 2) + pow(cn.y - c.y, 2));
			duration += c.duration;
			load += c.demand;
		}
		duration += sqrt(pow(cn.x - d.x, 2) + pow(cn.y - d.y, 2));
		load += cn.demand;

		if (duration > d.max_duration_per_vehicle && d.max_duration_per_vehicle > 0) punishment += 10 * (duration - d.max_duration_per_vehicle + 1) + 10;
		if (load > d.max_load_per_vehicle && d.max_load_per_vehicle > 0) punishment += 10 * (load - d.max_load_per_vehicle + 1) + 10;

		double fitness = duration + punishment;
		fitness_vehicle[index_individual][i] = fitness;
		individual_fitness += fitness;
	}
	fitness_individual[index_individual] = individual_fitness;
}

void Population::mutate_swap_internally_vehicle(std::vector<int> *individual, int index) {
	double fitness_before = this->get_fitness_individual(index);
	int random_vehicle;
	int random_loci_B = -1;
	int random_loci_A = -1;

	while (fitness_before == get_fitness_individual(individual)) {
		//std::cout << "started" << std::endl;
		random_vehicle = rand() % n_vehicles*n_depots;
		while (individual[random_vehicle].size() <= 2) {
			random_vehicle = rand() % n_vehicles*n_depots;
		}
		random_loci_A = rand() % individual[random_vehicle].size();

		do {
			random_loci_B = rand() % individual[random_vehicle].size();
		} while (random_loci_B == random_loci_A);
		std::swap(individual[random_vehicle][random_loci_A], individual[random_vehicle][random_loci_B]);
		//std::cout << random_loci_A << " " << random_loci_B << std::endl;
	}

}
void Population::insert_mutation_in_offspring(int n_mutate) {
	int mutated = 0;
	int offspring_index = n_parents;
	while (mutated < n_mutate) {
		int mutate = (rand() % 100);
		if (mutate < n_mutate) {
			mutate_insert_between_vehicle(population[offspring_index]);
			mutated++;
		}
		offspring_index++;
		if (offspring_index = this->n_parents + this->n_offspring) offspring_index = n_parents;
	}
}

void Population::insert_volatile_mutation_in_offspring(int n_mutate, int volatility) {
	for (int i = 0; i < n_mutate; i++) {
		int random_offspring = rand() % this->n_offspring;
		for (int j = 0; j < volatility; j++) {
			mutate_insert_between_vehicle(population[this->n_parents + random_offspring]);
		}
	}
}

void Population::insert_recombination_in_population_deterministic_pairing(std::set<int>* parent_index) {
	std::vector<int>* individual_A;
	std::vector<int>* individual_B;
	for (auto it = parent_index->begin(); it != parent_index->end();) { // Iterate it twice without going past end
		it++;
		if (it == parent_index->end()) continue;
		std::cout << "parent_index: " << *it-1 << std::endl;
		std::cout << "parent_index: " << *it << std::endl;
		individual_A = population[*it - 1];
		individual_B = population[*it];		for (int i = 0; i < 16; i++) {			std::cout << std::endl;			for (int j = 0; j < individual_A[i].size(); j++) {				std::cout << individual_A[i][j] << std::endl;			}		}		recombination_BCRC(individual_A, individual_B);		print_individual(*it - 1);		print_individual(*it);		it++;
	}
}

void Population::insert_recombination_in_population_random_pairing(std::set<int>* parent_index) {
	std::vector<int>* individual_A;
	std::vector<int>* individual_B;
	while (parent_index->size() > 1) {
		int rand_index_A = rand() % parent_index->size();
		int rand_index_B = rand_index_A;
		while (rand_index_A == rand_index_B) {
			rand_index_B = rand() % parent_index->size();
		}
		auto itA = parent_index->begin();
		auto itB = parent_index->begin();
		for (int i = 0; i < rand_index_A; i++) {
			itA++;
		}
		for (int i = 0; i < rand_index_B; i++) {
			itB++;
		}

		individual_A = population[*itA];
		individual_B = population[*itB];
		recombination_BCRC(individual_A, individual_B);
		parent_index->erase(itA);
		parent_index->erase(itB);
	}
}

void Population::remove_not_selected_members(std::set<int>* selected_index) { // selected indexes = the ones which survives selection
	std::vector<int> offspring_selected;
	std::vector<int> original_not_selected;

	//std::cout << "Individuals: " << n_individuals << ", size of selected: " << selected_index->size() << std::endl;
	//std::cout << "Originals removed: ";
	for (int i = 0; i < n_individuals + n_offspring; i++) {
		if (selected_index->find(i) == selected_index->end()) {
			if (i < n_individuals) {
				original_not_selected.push_back(i);
				//std::cout << i << " ";
			}
		}
		else {
			if (i >= n_individuals) {
				offspring_selected.push_back(i);
			}
		}
	}
	//std::cout << std::endl;

	if (offspring_selected.size() != original_not_selected.size())
		std::cout << "Different size: " << offspring_selected.size() << " " << original_not_selected.size() << std::endl;
	std::vector<int>* individual;
	for (int i = 0; i < offspring_selected.size(); i++) {
		int new_index = original_not_selected[i];
		for (int j = 0; j < n_vehicles*n_depots; j++) {
			population[new_index][j] = population[offspring_selected[i]][j];
		}
		this->fitness_individual_initalization(population[new_index], new_index);
	}
	this->n_offspring = 0;
}

double Population::get_best_fitness() {
	return this->best_fitness;
}

void Population::mutate_insert_between_vehicle(std::vector<int> *individual) {
	int random_vehicle_A = rand() % n_vehicles*n_depots;
	int random_vehicle_B;
	do {
		random_vehicle_B = rand() % n_vehicles*n_depots;
	} while (random_vehicle_B == random_vehicle_A);

	if (individual[random_vehicle_B].size() == 0) {
		if (individual[random_vehicle_A].size() == 0) return;
		int temp = random_vehicle_B;
		random_vehicle_B = random_vehicle_A;
		random_vehicle_A = temp;
	}
	
	int random_loci_A;
	if (individual[random_vehicle_A].size() == 0)  random_loci_A = 0;
	else random_loci_A = rand() % individual[random_vehicle_A].size();
	int random_loci_B = rand() % individual[random_vehicle_B].size();

	std::vector<int>::iterator it_A, it_B;
	it_A = individual[random_vehicle_A].begin() + random_loci_A;
	it_B = individual[random_vehicle_B].begin() + random_loci_B;
	individual[random_vehicle_A].insert(it_A, individual[random_vehicle_B][random_loci_B]);
	individual[random_vehicle_B].erase(it_B);
}

void Population::recombination_BCRC(std::vector<int> *parent_A, std::vector<int> *parent_B) {

	int random_depot = rand() % n_depots;
	int random_vehicle_A_index = (rand() % n_vehicles) + n_vehicles * random_depot;
	int random_vehicle_B_index = (rand() % n_vehicles) + n_vehicles * random_depot;

	std::vector<int>* offspring_A = population[n_individuals + n_offspring++];
	std::vector<int>* offspring_B = population[n_individuals + n_offspring++];
	//std::vector<int> vehicle_A_customer_buffer = ...; Should be Population member variable in order to allocate instead of reallocation over and over?
	//std::vector<int> vehicle_B_customer_buffer = ...; Same as ^
	//std::vector<double> cost_of_insertion_buffer = ...; Same as ^ 

	for (int vehicle_index = 0; vehicle_index < n_vehicles*n_depots; vehicle_index++) {
		offspring_A[vehicle_index] = parent_A[vehicle_index];
		offspring_B[vehicle_index] = parent_B[vehicle_index];
	}
	// Remove customers in vehicle_A_customer_buffer from offspring_B, and vica versa
	vehicle_A_customer_buffer = offspring_A[random_vehicle_A_index];
	vehicle_B_customer_buffer = offspring_B[random_vehicle_B_index];

	fitness_individual_initalization(offspring_A, n_offspring + n_individuals - 2);
	fitness_individual_initalization(offspring_B, n_offspring + n_individuals - 1);
	//std::cout << "Fitness before recombination, A: " << fitness_individual[n_offspring + n_individuals - 2] << ", B: " << fitness_individual[n_offspring + n_individuals - 2] << std::endl;

	/*std::cout << std::endl;
	std::cout << "Vehicle to be removed from A: " << random_vehicle_A_index << ", and B: " << random_vehicle_B_index << std::endl;
	std::cout << std::endl;*/
	/*static int i = 0;
	if (i == 0) {
		std::cout << "Vehicle to be removed from A: " << random_vehicle_A_index << ", and B: " << random_vehicle_B_index << std::endl;
		std::cout << std::endl;
		std::cout << "Offspring A:" << std::endl;
		print_individual(n_individuals + n_offspring - 2);
		std::cout << std::endl;
		std::cout << "Offspring B:" << std::endl;
		print_individual(n_individuals + n_offspring - 1);
	}*/
	/*
	for (int vehicle_index = 0; vehicle_index < n_depots*n_vehicles; vehicle_index++) {
		if (vehicle_B_customer_buffer.size() > 0 && offspring_A[vehicle_index].size() > 0) {
			for (int customer_index = 0; customer_index < offspring_A[vehicle_index].size(); customer_index++) {
				if (offspring_A[vehicle_index].size() <= customer_index) continue;
				for (int customer_buffer_i = 0; customer_buffer_i < vehicle_B_customer_buffer.size(); customer_buffer_i++) {
					if (offspring_A[vehicle_index][customer_index] == vehicle_B_customer_buffer[customer_buffer_i]) {
						// Remove customer at customer_index from offspring_A[vehicle_index]
						auto it = offspring_A[vehicle_index].begin() + customer_index;
						offspring_A[vehicle_index].erase(it);
						std::cout << "Her!" << std::endl;
					}
					//std::cout << "Løkke 1 " << vehicle_index << std::endl;
				}
			} // endfor
		} // endif
		if (vehicle_A_customer_buffer.size() > 0 && offspring_B[vehicle_index].size() > 0) {
			for (int customer_index = 0; customer_index < offspring_B[vehicle_index].size(); customer_index++) {
				if (offspring_B[vehicle_index].size() <= customer_index) continue;
				for (int customer_buffer_i = 0; customer_buffer_i < vehicle_A_customer_buffer.size(); customer_buffer_i++) {
					if (offspring_B[vehicle_index][customer_index] == vehicle_A_customer_buffer[customer_buffer_i]) {
						// Remove customer at customer_index from offspring_B[vehicle_index]
						auto it = offspring_B[vehicle_index].begin() + customer_index;
						offspring_B[vehicle_index].erase(it);
						std::cout << "Her!" << std::endl;
					}
					//std::cout << "Løkke 2 " << vehicle_index << std::endl;
				}
			} // endfor
		} // endif
	} */

	for (int vehicle_index = 0; vehicle_index < n_depots*n_vehicles; vehicle_index++) {
		if (vehicle_B_customer_buffer.size() > 0) {
			int check_for_removal_index = 0;
			while (check_for_removal_index < offspring_A[vehicle_index].size()) {
				for (int remove_if_found_index = 0; remove_if_found_index < vehicle_B_customer_buffer.size(); remove_if_found_index++) {
					if (offspring_A[vehicle_index][check_for_removal_index] == vehicle_B_customer_buffer[remove_if_found_index]) {
						// Remove customer at customer_index from offspring_A[vehicle_index]
						auto it = offspring_A[vehicle_index].begin() + check_for_removal_index;
						offspring_A[vehicle_index].erase(it);
						check_for_removal_index--;
						break;
					}
					//std::cout << "Løkke 1 " << vehicle_index << std::endl;
				}
				check_for_removal_index++;
			} // endfor
		} // endif
		if (vehicle_A_customer_buffer.size() > 0) {
			int check_for_removal_index = 0;
			while (check_for_removal_index < offspring_B[vehicle_index].size()) {
				for (int remove_if_found_index = 0; remove_if_found_index < vehicle_A_customer_buffer.size(); remove_if_found_index++) {
					if (offspring_B[vehicle_index][check_for_removal_index] == vehicle_A_customer_buffer[remove_if_found_index]) {
						// Remove customer at customer_index from offspring_B[vehicle_index]
						auto it = offspring_B[vehicle_index].begin() + check_for_removal_index;
						offspring_B[vehicle_index].erase(it);
						check_for_removal_index--;
						break;
					}
					//std::cout << "Løkke 2 " << vehicle_index << std::endl;
				}
				check_for_removal_index++;
			} // endfor
		} // endif
	}

	/*if (i == 0) {
		std::cout << std::endl;
		std::cout << "Offspring A:" << std::endl;
		print_individual(n_individuals + n_offspring - 2);
		std::cout << std::endl;
		std::cout << "Offspring B:" << std::endl;
		print_individual(n_individuals + n_offspring - 1);
	}*/

	// For the CHOSEN depot, compute insertion of vehicle_A_customer_buffer customers into all "FEASIBLE" points in random_depot (only), in offspring_B, and vica versa
	double lowest_cost_of_insertion = DBL_MAX;
	double fitness_of_insertion = 0;
	double fitness_pre_insertion = 0;
	double cost_of_insertion = 0;
	int index_of_best_insertion = 0;
	int vehicle_of_best_insertion = 0;
	int removed_customer_A_i = 0;
	int best_removed_customer_A_i = -1;
	while (removed_customer_A_i < vehicle_A_customer_buffer.size()) {

		for (int vehicle_index = n_vehicles * random_depot; vehicle_index < n_vehicles*random_depot + n_vehicles; vehicle_index++) {
			for (int offspring_customer_B_i = 0; offspring_customer_B_i < offspring_B[vehicle_index].size() + 1; offspring_customer_B_i++) { // Need to check insertion costs for empty vehicles, and insertion at the end of the route
				fitness_pre_insertion = get_fitness_vehicle(random_depot, offspring_B[vehicle_index]);
				std::vector<int>::iterator it;
				if (offspring_customer_B_i < offspring_B[vehicle_index].size()) it = offspring_B[vehicle_index].begin() + offspring_customer_B_i;
				else it = offspring_B[vehicle_index].end();
				it = offspring_B[vehicle_index].insert(it, vehicle_A_customer_buffer[removed_customer_A_i]);
				fitness_of_insertion = get_fitness_vehicle(random_depot, offspring_B[vehicle_index]);
				cost_of_insertion = fitness_of_insertion - fitness_pre_insertion;
				if (cost_of_insertion < lowest_cost_of_insertion) {
					lowest_cost_of_insertion = cost_of_insertion;
					index_of_best_insertion = offspring_customer_B_i;
					best_removed_customer_A_i = removed_customer_A_i;
					vehicle_of_best_insertion = vehicle_index;
					//std::cout << "Lowest insertion: " << lowest_cost_of_insertion << ", with in removed_index: " << removed_customer_A_i << ", vehicle: " << vehicle_index << std::endl;
				}
				offspring_B[vehicle_index].erase(it);
			}
		}
		if (removed_customer_A_i == vehicle_A_customer_buffer.size() - 1) {
			//std::cout << "\nNow inserting in B: " << vehicle_A_customer_buffer[best_removed_customer_A_i] << std::endl << std::endl;
			//std::cout << "V: " << vehicle_of_best_insertion << ", I: " << index_of_best_insertion << ", LC: " << lowest_cost_of_insertion << std::endl;
			auto it = offspring_B[vehicle_of_best_insertion].begin() + index_of_best_insertion;
			offspring_B[vehicle_of_best_insertion].insert(it, vehicle_A_customer_buffer[best_removed_customer_A_i]);

			//std::cout << best_removed_customer_A_i << std::endl;

			//std::cout << "\nNow removing from A_buffer: " << best_removed_customer_A_i << std::endl << std::endl;
			auto it2 = vehicle_A_customer_buffer.begin() + best_removed_customer_A_i;
			vehicle_A_customer_buffer.erase(it2);

			removed_customer_A_i = 0;
			lowest_cost_of_insertion = DBL_MAX;
			fitness_of_insertion = 0;
			fitness_pre_insertion = 0;
			cost_of_insertion = 0;
			index_of_best_insertion = 0;
			vehicle_of_best_insertion = 0;
		}
		else removed_customer_A_i++;
	}
	lowest_cost_of_insertion = DBL_MAX;
	fitness_of_insertion = 0;
	fitness_pre_insertion = 0;
	cost_of_insertion = 0;
	index_of_best_insertion = 0;
	vehicle_of_best_insertion = 0;
	int insert_removed_customer_B_i_next = -1;
	int removed_customer_B_i = 0;
	int best_removed_customer_B_i = -1;
	while(removed_customer_B_i < vehicle_B_customer_buffer.size()) {

		for (int vehicle_index = n_vehicles * random_depot; vehicle_index < n_vehicles*random_depot + n_vehicles; vehicle_index++) {
			for (int offspring_customer_A_i = 0; offspring_customer_A_i < offspring_A[vehicle_index].size() + 1; offspring_customer_A_i++) { // Need to check insertion costs for empty vehicles, and insertion at the end of the route
				fitness_pre_insertion = get_fitness_vehicle(random_depot, offspring_B[vehicle_index]);

				std::vector<int>::iterator it;
				if (offspring_customer_A_i < offspring_A[vehicle_index].size()) it = offspring_A[vehicle_index].begin() + offspring_customer_A_i;
				else it = offspring_A[vehicle_index].end();

				it = offspring_A[vehicle_index].insert(it, vehicle_B_customer_buffer[removed_customer_B_i]);
				fitness_of_insertion = get_fitness_vehicle(random_depot, offspring_A[vehicle_index]);
				cost_of_insertion = fitness_of_insertion - fitness_pre_insertion;
				if (cost_of_insertion < lowest_cost_of_insertion) {
					lowest_cost_of_insertion = cost_of_insertion;
					index_of_best_insertion = offspring_customer_A_i;
					best_removed_customer_B_i = removed_customer_B_i;
					vehicle_of_best_insertion = vehicle_index;
					//std::cout << "Lowest insertion: " << lowest_cost_of_insertion << ", with in removed_index: " << removed_customer_B_i << ", vehicle: " << vehicle_index << std::endl;
				}
				offspring_A[vehicle_index].erase(it);
			}
		}
		if (removed_customer_B_i == vehicle_B_customer_buffer.size() - 1) {
			//std::cout << "\nNow inserting in A: " << vehicle_B_customer_buffer[best_removed_customer_B_i] << std::endl << std::endl;
			//std::cout << "V: " << vehicle_of_best_insertion << ", I: " << index_of_best_insertion << ", LC: " << lowest_cost_of_insertion << std::endl;
			auto it = offspring_A[vehicle_of_best_insertion].begin() + index_of_best_insertion;
			offspring_A[vehicle_of_best_insertion].insert(it, vehicle_B_customer_buffer[best_removed_customer_B_i]);

			//std::cout << best_removed_customer_A_i << std::endl;

			//std::cout << "\nNow removing from B_buffer: " << best_removed_customer_B_i << std::endl << std::endl;
			auto it2 = vehicle_B_customer_buffer.begin() + best_removed_customer_B_i;
			vehicle_B_customer_buffer.erase(it2);

			removed_customer_B_i = 0;
			lowest_cost_of_insertion = DBL_MAX;
			fitness_of_insertion = 0;
			fitness_pre_insertion = 0;
			cost_of_insertion = 0;
			index_of_best_insertion = 0;
			vehicle_of_best_insertion = 0;
		}
		else removed_customer_B_i++;
	}
	fitness_individual_initalization(offspring_A, n_offspring + n_individuals - 2);
	fitness_individual_initalization(offspring_B, n_offspring + n_individuals - 1);

	/*if (i == 0) {
		std::cout << std::endl;
		std::cout << "Offspring A:" << std::endl;
		print_individual(n_individuals + n_offspring - 2);
		std::cout << std::endl;
		std::cout << "Offspring B:" << std::endl;
		print_individual(n_individuals + n_offspring - 1);
		std::cin.get();
		exit(1);
		i++;
	}*/

	//std::cout << "Fitness after recombination, A: " << fitness_individual[n_offspring + n_individuals - 2] << ", B: " << fitness_individual[n_offspring + n_individuals - 2] << std::endl << std::endl;

	validate_individual(n_individuals + n_offspring - 1);
	validate_individual(n_individuals + n_offspring - 2);
}


bool operator<(const fitness_index &right, const fitness_index &left) {
	return right.fitness < left.fitness;
}




void Population::selection_ellitisme(int n_ellitisme, std::set<int>& survival_index, selection_on selection_type) {
	static int error = 0;
	if (error == 0 && n_ellitisme == 0) {
		std::cout << "Warning, number of elitism set to zero." << std::endl;
		error++;
		return;
	}
	
	
	std::set<fitness_index> temp;
	int n_individ;
	if (selection_type == parent_selection) {
		n_individ = this->n_individuals;
	}
	else {
		n_individ = this->n_offspring + this->n_individuals;
	}

	double l_b = DBL_MAX;
	double u_b = DBL_MAX;
	for (int i = 0; i < n_individ; i++) {
		if (temp.size() < n_ellitisme) {
			temp.insert(fitness_index(i, fitness_individual[i]));

			if (fitness_individual[i] < l_b) {
				l_b = fitness_individual[i];
			}
			/*for (auto it = temp.begin(); it != temp.end(); it++) {
				std::cout << "F: " << it->fitness << std::endl;
			}
			std::cout << std::endl;*/
			u_b = (temp.begin())->fitness;
		}
		else if (this->fitness_individual[i] < u_b) {
			
			auto it = temp.end();
			it--;
			temp.erase(it);
			temp.insert(fitness_index(i, fitness_individual[i]));
			
			if (fitness_individual[i] < l_b) {
				l_b = fitness_individual[i];
			}
			/*for (auto it = temp.begin(); it != temp.end(); it++) {
				std::cout << "F: " << it->fitness << std::endl;
			}*/
			u_b = (temp.rbegin())->fitness;
		}
	}
	//std::cout << "This fitness is: " << this->best_fitness << std::endl;
	this->best_fitness = l_b;

	for (auto it = temp.begin(); it != temp.end(); it++) {
		survival_index.insert(it->index);
	}
}

void Population::selection_SUS(int n_pointers, std::set<int>& survival_index, selection_on selection_type) {

	int n_individ;
	int individ_start;
	if (selection_type == parent_selection) {
		n_individ = this->n_individuals;
		individ_start = 0;
	}
	else if (selection_type == offspring_selection) {
		n_individ = this->n_offspring;
		individ_start = this->n_individuals;
	}
	else if (selection_type == population_selection) {
		n_individ = this->n_offspring + this->n_individuals;
		individ_start = 0;
	}

	// make wheel
	double sum_valid_fitness = 0;
	double n_valid_individuals = n_individ - survival_index.size();
	wheel_piece_t* wheel = new wheel_piece_t[n_valid_individuals]; // n = number of population taken care of
	int index_wheel = 0;
	for (int i = individ_start; i < n_individ; i++) {
		if (survival_index.find(i) == survival_index.end()) {
			sum_valid_fitness += 1 / fitness_individual[i];
			wheel[index_wheel] = wheel_piece_t(0, 0, i);
			index_wheel++;
		}
	}

	double l_b = 0;
	for (int i = 0; i < n_valid_individuals; i++) {
		double f = (1 / fitness_individual[wheel[i].individual_index]) / sum_valid_fitness;
		wheel[i].l_b = l_b;
		wheel[i].u_b = l_b+f;
		l_b += f;
	}
	/*std::cout << "wheel" << std::endl;
	for (int i = 0; i < n_valid_individuals; i++) {

	std::cout <<"Index: " << wheel[i].individual_index << " lb: " << wheel[i].l_b << " ub: " << wheel[i].u_b << std::endl;
	}*/

	double step_size = 1.0 / n_pointers;
	double p_start = (rand() % (100 + 1)) / 100.0;

	// roulette wheel
	int n_curr_offspring = 0;
	double pointer = p_start;
	int i = 0;
	int j = 0;
	while (n_curr_offspring < n_pointers) {
		if (wheel[i].l_b <= pointer && wheel[i].u_b >= pointer) {
			//std::cout << std::fixed;
			//std::cout << "Pointer " << pointer << " individual " << std::setprecision(4) << wheel[i].l_b << " " << std::setprecision(4) << wheel[i].u_b << std::endl;
			survival_index.insert(wheel[i].individual_index);
			pointer += step_size;
			n_curr_offspring += 1;
			if (pointer > 1) pointer = pointer - 1;
		}
		i++;
		if (pointer < 0 || pointer > 1) std::cout << "SUS wheel pointer out of range: " << pointer << std::endl;
		if (i >= n_valid_individuals) {
			i = 0;
			j++;
		}
		if (j >= n_individuals) {
			std::cout << "Stuck in loop. n_Chosen individuals: " << n_curr_offspring << ", should have selected: " << n_pointers << std::endl;
			break;
		}
	}
	delete[] wheel;
}