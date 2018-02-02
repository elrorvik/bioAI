#include<iostream>
#include"decode.h"

bool test_ONOFF = true;

double placeholder_fitness(Individual ind) {
    double fitness = 0;
    int i  = 0;

    city c = get_city(0);
    city cn = c;

    fitness += (c.x-ind.d.x)^2 + (c.y-ind.d.y)^2;

    while( i++ < n_customers-1) {
        c = cn;
        cn = get_city(i+1);
        fitness += (cn.x-c.x)^2 + (cn.y - c.y)^2;
    }

    

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