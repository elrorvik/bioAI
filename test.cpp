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

double fitness_vehicle(int depot_number, std::vector<int> vehicle) {
    double duration = 0;
    double load = 0;
    double fitness = 0;
    double punishment = 0;

    customer c = vehicle[0];
    customer cn = c;
    depot d = depots[depot_number];

    duration += sqrt(pow(c.x - d.x, 2) + pow(c.y - d.y, 2));
    for (int j = 1; j < individual[i].size(); j++) {
        c = cn;
        cn = vehicle[j];
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

void recombination_BCRC(std::vector<int> *parent_A, std::vector<int> *parent_B) {
    
    int random_depot = rand() % n_depot;
    int random_vehicle_A_index = (rand() % n_vehicles) + n_vehicles*random_depot;
	int random_vehicle_B_index = (rand() % n_vehicles) + n_vehicles*random_depot;

    std::vector<int>* offspring_A = offspring[n_offspring++];
	std::vector<int>* offspring_B = offspring[n_offspring++];
    //std::vector<int> vehicle_A_customer_buffer = ...; Should be Population member variable in order to allocate instead of reallocation over and over?
    //std::vector<int> vehicle_B_customer_buffer = ...; Same as ^
    //std::vector<double> cost_of_insertion_buffer = ...; Same as ^ 

    for (int i = 0; i < n_vehicles*n_depots; i++) {
        offspring_A[i] = parent_A[i];
        offspring_B[i] = parent_B[i];
    }

    // Remove customers in vehicle_A_customer_buffer from offspring_B, and vica versa
    vehicle_A_customer_buffer = offspring_A[random_vehicle_A_index];
    vehicle_B_customer_buffer = offspring_B[random_vehicle_B_index];
    for (int vehicle_i = 0; vehicle_i < n_depots*n_vehicles; vehicle_i++) {
        if (vehicle_B_customer_buffer.size()) {
        for (int customer_i = 0; customer_i < offspring_A[vehicle_i].size(); customer_i++) {
            for (int customer_buffer_i = 0; customer_buffer_i < vehicle_B_customer_buffer; customer_buffer_i++) {
                if (offspring_A[vehicle_i][customer_i] == vehicle_B_customer_buffer[customer_buffer_i]) {
                    // Remove customer at customer_i from offspring_A[vehicle_i]
                    auto it = offspring_A[vehicle_i].begin() + customer_i;
                    offspring_A[vehicle_i].erase(it);
                }
            }
        } // endfor
        } // endif
        if (vehicle_A_customer_buffer.size()) {
        for (int customer_i = 0; customer_i < offspring_B[vehicle_i].size(); customer_i++) {
            for (int customer_buffer_i = 0; customer_buffer_i < vehicle_A_customer_buffer; customer_buffer_i++) {
                if (offspring_B[vehicle_i][customer_i] == vehicle_A_customer_buffer[customer_buffer_i]) {
                    // Remove customer at customer_i from offspring_B[vehicle_i]
                    auto it = offspring_B[vehicle_i].begin() + customer_i;
                    offspring_B[vehicle_i].erase(it);
                }
            }
        } // endfor
        } // endif
    }

    // For the CHOSEN depot, compute insertion of vehicle_A_customer_buffer customers into all "FEASIBLE" points in random_depot (only), in offspring_B, and vica versa
    for (int removed_customer_A_i = 0; removed_customer_A_i < vehicle_A_customer_buffer.size(); removed_customer_A_i++) {
        double lowest_cost_of_insertion = DBL_MAX;
        double fitness_of_insertion = 0;
        double fitness_pre_insertion = 0;
        double cost_of_insertion = 0;
        int index_of_best_insertion = 0;
        int vehicle_of_best_insertion = 0;
        for (int vehicle_i = n_vehicles*random_depot; vehicle_i < n_vehicles*random_depot + n_vehicles; vehicle_i++) {
            for (int offspring_customer_B_i = 0; offspring_customer_B_i < offspring_B[vehicle_i*random_depot].size() + 1; offspring_customer_B_i++) { // Need to check insertion costs for empty vehicles, and insertion at the end of the route
                fitness_pre_insertion = fitness_vehicle(random_depot, offspring_B[vehicle_i]);
                auto it = offspring_B.begin() + offspring_customer_B_i;
                offspring_B[vehicle_i].insert(it, vehicle_A_customer_buffer[removed_customer_A_i]);
                fitness_of_insertion = fitness_vehicle(random_depot, offspring_B[vehicle_i]);
                cost_of_insertion = fitness_of_insertion - fitness_pre_insertion;
                if (cost_of_insertion < lowest_cost_of_insertion) {
                    lowest_cost_of_insertion = cost_of_insertion;
                    index_of_best_insertion = offspring_customer_B_i;
                    vehicle_of_best_insertion = vehicle_i;
                }
                offspring_B[vehicle_i].erase(it);
            }
        }
        auto it = offspring_B.begin() + index_of_best_insertion;
        offspring_B[vehicle_of_best_insertion].insert(it, vehicle_A_customer_buffer[removed_customer_A_i])
    }
    for (int removed_customer_B_i = 0; removed_customer_B_i < vehicle_A_customer_buffer.size(); removed_customer_B_i++) {
        double lowest_cost_of_insertion = DBL_MAX;
        double fitness_of_insertion = 0;
        double fitness_pre_insertion = 0;
        double cost_of_insertion = 0;
        int index_of_best_insertion = 0;
        int vehicle_of_best_insertion = 0;
        for (int vehicle_i = n_vehicles*random_depot; vehicle_i < n_vehicles*random_depot + n_vehicles; vehicle_i++) {
            for (int offspring_customer_A_i = 0; offspring_customer_A_i < offspring_A[vehicle_i].size() + 1; offspring_customer_A_i++) { // Need to check insertion costs for empty vehicles, and insertion at the end of the route
                fitness_pre_insertion = fitness_vehicle(random_depot, offspring_B[vehicle_i]);
                auto it = offspring_A.begin() + offspring_customer_A_i;
                offspring_A[vehicle_i].insert(it, vehicle_A_customer_buffer[removed_customer_B_i]);
                fitness_of_insertion = fitness_vehicle(random_depot, offspring_A[vehicle_i]);
                cost_of_insertion = fitness_of_insertion - fitness_pre_insertion;
                if (cost_of_insertion < lowest_cost_of_insertion) {
                    lowest_cost_of_insertion = cost_of_insertion;
                    index_of_best_insertion = offspring_customer_A_i;
                    vehicle_of_best_insertion = vehicle_i;
                }
                offspring_A[vehicle_i].erase(it);
            }
        }
        auto it = offspring_A.begin() + index_of_best_insertion;
        offspring_A[vehicle_of_best_insertion].insert(it, vehicle_B_customer_buffer[removed_customer_B_i])
    }
    //TODO: "Feasibility" er ikke brydd om fordi dette er et grådig søk uansett - har vi tenkt å bry oss noe om det?
    //TODO: Bør antakelig skrive noen hjelpefunksjoner for å gjøre dette lesbart. Også - sjekk at det funker.
    // Det jeg tenker er interne funksjoner bare for å gjøre koden lesbar egentlig.
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