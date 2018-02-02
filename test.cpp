#include<iostream>
#include<cmath>
#include"decode.h"

bool test_ONOFF = true;

double placeholder_fitness_individual(Individual ind) {
    double fitness = 0;
    double duration = 0;
    double load = 0;
    double punishment = 0;

    city c = ind.get_city(0);
    city cn = c;

    duration += sqrt((c.x-ind.d.x)^2 + (c.y-ind.d.y)^2);
    for(int i = 0; i < n_customers-1; i++) {
        c = cn;
        cn = ind.get_city(i+1);
        duration += sqrt((cn.x-c.x)^2 + (cn.y - c.y)^2);
        duration += c.duration;
        load += c.load;
    }
    duration += sqrt((cn.x-ind.d.x)^2 + (cn.y-ind.d.y)^2);
    load += cn.load;

    if(duration > ind.d.max_duration_per_vehicle) punishment += 500;
    if(load > ind.d.max_load_per_vehicle) punishment += 500;
    fitness = duration + punishment;

    return fitness;
}

double placeholder_fitness_total(Population pop) { //Blir vel i så fall en litt meningsløs funksjon.
    double fitness;
    for each individual in population:
        fitness = placeholder_fitness_individual(individual);
    
    if not all cities visited: // Dette er vel umulig i utgangspunktet
        cout << "Illegal configuration" << endl;
        exit(1);
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