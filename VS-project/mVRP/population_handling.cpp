#include <iostream>
#include "population_hadling.h"
#include "mVRP.h"



void initialize_population(customer **population, int n_vehicles, int n_customers, int n_depots, std::set<costumer> &costumers, std::set<depot> &depots) {
	


}

void allocate_memory_population(customer **population, int n_vehicles, int n_customers, int n_depots, int n_population) {
	population = new customer*[n_population];
	
		for (int i = 0; i < n_population; i++)
		{
			population[i] = new costumer[n_vehicles*n_customers];
			for (int j = 0; j < n_vehicles*n_customers) {
				population[i][j] = { invalid_city,0,0,0,0 };
			}
		}


}