#include<iostream>
#include<cmath>
#include"decode.h"

bool test_ONOFF = true;



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