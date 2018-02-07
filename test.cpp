#include<iostream>
#include<cmath>
#include"decode.h"

bool test_ONOFF = true;

/*
void crossover_edge(std:vector<int> *parent_A, std::vector<int> *parent_B) {

    int vehicle_A = (rand() % n_vehicles) + n_vehicles*depot;
	int vehicle_B;
	do {
		(rand() % n_vehicles) + n_vehicles*depot
	} while (vehicle_B == vehicle_A);

    int* nodes_added_to_offspring = new int[n_customers];
    int* nodes_not_added_to_offspring = new int[n_customers];
    int length_A = parent_A[vehicle_A].size();
    int length_B = parent_B[vehicle_B].size();
    set<set<int>> edge_table;

    // Deal with the depot as an element in the cyclic list
    int depot_node = n_customers; // Give the node a unique identifier
    edge_table;

    
    for (int i = 0; i < length_A+length_B) {
        delete edge_table[i];
    }
    delete left_out [];
    delete left_in [];
    
}
*/

void recombination_BCRC(std::vector<int> *parent_A, std::vector<int> *parent_B) {
    
    std::vector<int>* offspring_A = new std::vector<int>[n_vehicles];
	std::vector<int>* offspring_B = new std::vector<int>[n_vehicles];
    for (int i = 0; i < n_vehicles; i++) {
        offspring_A[i] = parent_A[i];
        offspring_B[i] = parent_B[i];
    }

    int random_vehicle_A_index = (rand() % n_vehicles) + n_vehicles*depot;
	int random_vehicle_B_index = (rand() % n_vehicles) + n_vehicles*depot;
    int* vehicle_A = new int[offspring_A[random_vehicle_A_index].size()];
    int* vehicle_B = new int[offspring_B[random_vehicle_B_index].size()];
}

double distance(const customer ca, const customer cb) {
    return sqrt((ca.x - cb.x)^2 + (ca.y-cb.y)^2);
}

double distance(const depot d, const customer c) {
    return sqrt((d.x - c.x)^2 + (d.y-c.y)^2);
}

std::vector<double> * fitness;
void SUS_selection(std::vector<int> ** potential_parents, int number_of_parents) {
    
}

double Population::fitness_vehicle(std::vector<int> *individual, int vehicle) {
	double fitness = 0;
	double duration = 0;
	double load = 0;
	double punishment = 0;

	/*Fitness is "duration of service" = travel distance
	+ punishment of not satisfying all customers on a route
	+ punishment of a route being too long*/
    customer c = get_customer(individual[vehicle][0]);
    customer cn = c;
    depot d = depots[static_cast<int>(std::floor(vehicle / n_vehicles + 0.001 / n_vehicles))];
    std::cout << static_cast<int>(std::floor(vehicle / n_vehicles + 0.001 / n_vehicles)) << std::endl;

    duration += sqrt(pow(c.x - d.x, 2) + pow(c.y - d.y, 2));
    for (int j = 0; j < individual[vehicle].size(); j++) {  
        c = cn;
        cn = get_customer(individual[vehicle][j]);
        duration += sqrt(pow(cn.x - c.x, 2) + pow(cn.y - c.y, 2));
        duration += c.duration;
        load += c.demand;
    }
    duration += sqrt(pow(cn.x - d.x, 2) + pow(cn.y - d.y, 2));
    load += cn.demand;

    if (duration > d.max_duration_per_vehicle) punishment += 500;
    if (load > d.max_load_per_vehicle) punishment += 500;
    fitness = duration + punishment;

    return fitness;
}

int main(){

    string filename = "Data Files\\p01";
    std::set<customer> customers;
    std::set<depot> depots;

    int n_vehicles;
    int n_customers;
    int n_depots;

    cout << "Hello world!\n";

    read_data(filename, customers, depots, n_vehicles, n_customers, n_depots);

    int i = 0;
    set<depot>::iterator d_iter = depots.begin();
    while( i++ < n_depots) {
        cout << d_iter->x << endl;
        d_iter++;
    }

    cin.get();

}