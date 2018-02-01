#pragma once
#include<string>
#include<set>
#include<fstream>
using namespace std;

extern bool test_ONOFF;

struct city {
    int index;
    int x;
    int y;
    int duration;
    int demand;
};
bool operator> (const city &right, const city &left) {
    return (right.x + right.y > left.x + left.y);
}

struct depot{
    int max_duration_per_vehicle;
    int max_load_per_vehicle;
    int x;
    int y;
};

bool operator> (const depot &right, const depot &left) {
    return (right.x + right.y > left.x + left.y);
}

void foo(string filename, set<city> cities, set<depot> depots, int &n_vehicles, int &n_customers, int &n_depots) {
    ifstream data_file;
    data_file.open(filename);

    if (!data_file) {
        cout << "Unable to open file!";
        exit(1);
    }

    int i = 0;

    data_file >> n_vehicles >> n_customers >> n_depots;
    if(test_ONOFF) cout << n_vehicles << " " << n_customers << " " << n_depots << endl;
    while (i++ < n_depots) {
        int max_duration_per_vehicle, max_load_per_vehicle;
        data_file >> max_duration_per_vehicle >> max_load_per_vehicle;
        //if(test_ONOFF) cout << max_duration_per_vehicle << " " << max_load_per_vehicle << endl;
        depot d = {max_duration_per_vehicle, max_load_per_vehicle, 0, i}; // i is to differentiate (required by set), x and y will be initialized below
        //depots.insert(d); //USE EMPLACE INSTEAD
    }

    i = 0;
    while (i++ < n_customers) {
        int index, x, y, duration, demand, dummy;
        data_file >> index >> x >> y >> duration >> demand >> dummy >> dummy >> dummy >> dummy >> dummy;
        //if(test_ONOFF) cout << index << " " << x << " " << y << " " << duration << " " << demand << endl;
        city c = {index, x, y, duration, demand};
        //cities.insert(c); //USE EMPLACE INSTEAD
    }
    cout << "i is: " << i << endl; //DEBUG

    
    i = 0;
    //set<int>::iterator iter = depots.begin();
    while (i++ < n_depots) {
        int x, y, dummy;
        data_file >> dummy >> x >> y >> dummy >> dummy >> dummy >> dummy;
        if(test_ONOFF) cout << x << " " << y << endl;
        //d = *iter;

    }
    cout << "i is: " << i << endl; //DEBUG
    
    data_file.close();
}