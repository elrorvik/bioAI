#include <iostream>
#include "population.h"
#include "mVRP.h"


Population::Population(int n_vehicles, int n_customers, int n_depots,int n_individuals, std::set<customer> &costumers, std::set<depot> &depots){
	this->population = new customer*[n_individuals];
	this->n_customers = n_customers;
	this->n_depots = n_depots;
	this->n_individuals = n_individuals;
	
	for (int i = 0; i < n_individuals; i++)
	{
		this->population[i] = new customer[n_vehicles*n_customers];
		for (int j = 0; j < n_vehicles*n_customers;) {
			this->population[i][j] = { invalid_city,0,0,0,0 };
		}
	}


}

Population::~Population() {

	for (int i = 0; i < this->n_individuals; i++)
	{
		delete [] this->population[i] ;
	}
	delete[] this->population;

}