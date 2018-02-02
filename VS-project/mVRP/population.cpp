#include <iostream>
#include "population.h"
#include "mVRP.h"
#include <time.h> 
#include <vector>


Population::Population(int n_vehicles, int n_customers, int n_depots, int n_individuals, std::set<customer> &customers, std::set<depot> &depots){
	this->population = new int*[n_individuals];
	this->n_customers = n_customers;
	this->n_depots = n_depots;
	this->n_individuals = n_individuals;
	this->n_vehicles = n_vehicles;

	std::cout <<"customers: " <<  this->n_customers << ", depots: " << this->n_depots << " individuals: " << this->n_individuals << " vehicles: " << this->n_vehicles<< std::endl;
	
	for (int i = 0; i < n_individuals; i++)
	{
		this->population[i] = new int[n_vehicles*n_customers];
		for (int j = 0; j < n_vehicles*n_customers;j++) {
			this->population[i][j] = invalid_city;
		}
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
		std::vector<int> available_customer_indexes;
		std::vector<int> available_individual_indexes;
		for (int i = 0; i < n_customers; i++) available_customer_indexes.push_back(i);
		for (int i = 0; i < n_customers*n_vehicles; i++) available_individual_indexes.push_back(i);

		for (int i = 0; i < n_customers; i++) {
			int random_index = rand() % available_customer_indexes.size();
			int customer_index = available_customer_indexes[random_index];
			available_customer_indexes.erase(available_customer_indexes.begin() + random_index);
			random_index = rand() % available_individual_indexes.size();
			int gene_index = available_individual_indexes[random_index];
			this->population[individual_index][gene_index] = customer_index;
			available_individual_indexes.erase(available_individual_indexes.begin() + random_index);
		}
		
		
	}
	
}

void Population::print_population() {

	for (int individual_index = 0; individual_index < n_individuals; individual_index++) {
		std::cout << "*** Individual. " << individual_index <<  " ***"<<std::endl;
		for (int i = 0; i < this->n_vehicles; i++) {
			this->print_vehicles_customer_queue(individual_index, i);
		}
	}
}

customer Population::get_customer(int index){
	return this->customers[index];
}

customer Population::get_customer(int individual_index, int vehicle_index,int customer_index_in_queue) {
	int customer_index = this->population[individual_index][vehicle_index*n_customers + customer_index_in_queue];
	return this->get_customer(customer_index);
}

void Population::print_vehicles_customer_queue(int individual_index, int vehicle_index) {
	int sum = 0;
	std::cout << "Vehicle nr. " << vehicle_index << "queue: ";
	for (int i = 0; i < n_customers; i++) {
		int customer_index = this->population[individual_index][vehicle_index*n_customers + i];
		/*if (customer_index == invalid_city) {
			continue;
		}*/
		std::cout << customer_index << " ";
		
	}
	std::cout << std::endl;
	
}