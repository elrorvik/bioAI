#include <iostream>
#include "population.h"
#include "mVRP.h"


Population::Population(int n_vehicles, int n_customers, int n_depots, int n_individuals, std::set<customer> &customers, std::set<depot> &depots){
	this->population = new int*[n_individuals];
	this->n_customers = n_customers;
	this->n_depots = n_depots;
	this->n_individuals = n_individuals;
	
	for (int i = 0; i < n_individuals; i++)
	{
		this->population[i] = new int[n_vehicles*n_customers];
		for (int j = 0; j < n_vehicles*n_customers;) {
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

	/*for (int i = 0; i < n_customers; i++) {
		std::cout << customers[i]
	}*/

}

Population::~Population() {

	for (int i = 0; i < this->n_individuals; i++)
	{
		delete[] this->population[i];
	}
	delete[] this->population;

}

void Population::initialize_random() {



}

