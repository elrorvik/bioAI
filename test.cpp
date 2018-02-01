#include<iostream>
#include"decode.h"

bool test_ONOFF = true;

int main(){

    string filename = "Data Files\\p01";
    std::set<city> cities;
    std::set<depot> depots;

    int n_vehicles;
    int n_customers;
    int n_depots;

    cout << "Hello world!\n";

    foo(filename, cities, depots, n_vehicles, n_customers, n_depots);

    cin.get();

}