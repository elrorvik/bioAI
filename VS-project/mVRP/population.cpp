#include <iostream>
#include "population.h"
#include "mVRP.h"
#include <time.h> 
#include <vector>
#include <set>


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
	srand(time(NULL)); // initialize random seed
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