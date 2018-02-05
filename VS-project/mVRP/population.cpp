#include <iostream>
#include "population.h"
#include "mVRP.h"
#include <time.h> 
#include <vector>
#include <set>
#include <utility>
#include <cmath>

Population::Population(int n_vehicles, int n_customers, int n_depots, int n_individuals, std::set<customer> &customers, std::set<depot> &depots){
	this->population = new std::vector<int>*[n_individuals];
	this->n_customers = n_customers;
	this->n_depots = n_depots;
	this->n_individuals = n_individuals;
	this->n_vehicles = n_vehicles;

	std::cout <<"customers: " <<  this->n_customers << ", depots: " << this->n_depots << " individuals: " << this->n_individuals << " vehicles: " << this->n_vehicles<< std::endl;
	
	for (int i = 0; i < n_individuals; i++)
	{
		this->population[i] = new std::vector<int>[n_vehicles*n_depots];
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

	for (int i = 0; i < this->n_individuals; i++)
	{
		delete[] this->population[i];
	}
	delete[] this->population;

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
	}
}

void Population::print_population() {

	for (int individual_index = 0; individual_index < n_individuals; individual_index++) {
		std::cout << "*** Individual. " << individual_index <<  " ***"<<std::endl;
		for (int depot_index = 0; depot_index < n_depots; depot_index++) {
			std::cout << "Depot nr.: " << std::endl;
			for (int vehicle_index = 0; vehicle_index < n_vehicles; vehicle_index++) {
				this->print_vehicles_customer_queue(individual_index, vehicle_index, depot_index);
			}
		}
	}
}

customer Population::get_customer(int index){
	if (index >= n_customers || index < 0) {
		return customer();
	}
	return this->customers[index];
}
int Population::get_depot_vehicle_index(int vehicle_index,int depot_index) {
	return this->n_vehicles*depot_index + vehicle_index;
}

customer Population::get_customer(int individual_index, int depot_index, int vehicle_index, int customer_index_in_queue) {
	int customer_index = this->population[individual_index][get_depot_vehicle_index(vehicle_index,depot_index)][customer_index_in_queue];
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

void Population::print_vehicles_customer_queue(int individual_index, int vehicle_index, int depot_index) {
	int sum = 0;
	std::cout << "Vehicle nr. " << vehicle_index << "queue: ";
	for (int i = 0; i < get_vehicle_queue_size(individual_index, depot_index, vehicle_index); i++) {
		int customer_index = this->get_customer_index(individual_index,depot_index,vehicle_index,i);
		
		std::cout << customer_index << " ";
	}
	std::cout << std::endl;
	
}

double Population::fitness_individual(std::vector<int> *individual) {
	double fitness = 0;
	double duration = 0;
	double load = 0;
	double punishment = 0;

	/*Fitness is "duration of service" = travel distance
	+ punishment of not satisfying all customers on a route
	+ punishment of a route being too long*/
	for (int i = 0; i < n_vehicles*n_depots; i++) {

		customer c = get_customer(individual[i][0]);
		customer cn = c;
		depot d = depots[static_cast<int>(std::floor(i / n_vehicles + 0.001 / n_vehicles))];

		duration += sqrt((c.x - d.x) ^ 2 + (c.y - d.y) ^ 2);
		for (int j = 0; j < individual[i].size(); j++) {
			c = cn;
			cn = get_customer(individual[i][j]);
			duration += sqrt((cn.x - c.x) ^ 2 + (cn.y - c.y) ^ 2);
			duration += c.duration;
			load += c.demand;
		}
		duration += sqrt((cn.x - d.x) ^ 2 + (cn.y - d.y) ^ 2);
		load += cn.demand;

		if (duration > d.max_duration_per_vehicle) punishment += 500;
		if (load > d.max_load_per_vehicle) punishment += 500;
		fitness = duration + punishment;
	}

	return fitness;
}

void Population::mutate_swap_internally_vehicle(std::vector<int> *individual) {
	int random_vehicle = rand() % n_vehicles*n_depots;
	int random_loci_A = rand() % individual[random_vehicle].size();
	int random_loci_B;
	do {
		random_loci_B = rand() % individual[random_vehicle].size();
	} while (random_loci_B != random_loci_A);

	std::swap(individual[random_vehicle][random_loci_A], individual[random_vehicle][random_loci_B]);
}

void Population::mutate_insert_between_vehicle(std::vector<int> *individual) {
	int random_vehicle_A = rand() % n_vehicles*n_depots;
	int random_vehicle_B = rand() % n_vehicles*n_depots;
	int random_loci_A = rand() % individual[random_vehicle_A].size();
	int random_loci_B = rand() % individual[random_vehicle_B].size();

	std::vector<int>::iterator it_A, it_B;
	it_A = individual[random_vehicle_A].begin() + random_loci_A;
	it_B = individual[random_vehicle_B].begin() + random_loci_B;

	individual[random_vehicle_A].insert(it_A, individual[random_vehicle_B][random_loci_B]);
	individual[random_vehicle_B].erase(it_B);
}

// void crossover_edge(std:vector<int>)