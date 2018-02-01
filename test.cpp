#include<iostream>
#include"decode.h"

bool test_ONOFF = true;

int main(){

    string filename = "Data Files\\p01";
    std::set<customer> customers;
    std::set<depot> depots;

    int n_vehicles;
    int n_customers;
    int n_depots;

    cout << "Hello world!\n";

    fetch_data(filename, customers, depots, n_vehicles, n_customers, n_depots);

    
    int i = 0;

    set<depot>::iterator d_iter = depots.begin();
    while( i++ < n_depots) {
        cout << d_iter->x << endl;
        d_iter++;
    }

    cin.get();

}