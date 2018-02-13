#include"file.h"
#include <iostream>
#include<fstream>

bool test_ONOFF = false;

void read_data(const string filename, set<customer> &customers, set<depot> &depots, int &n_vehicles, int &n_customers, int &n_depots) {
    ifstream data_file;
    data_file.open(filename);

    if (!data_file) {
        cout << "Unable to open file!";
        exit(1);
    }

    int i = 0;
    set<depot> temp_depots;
    data_file >> n_vehicles >> n_customers >> n_depots;
    if(test_ONOFF) cout << n_vehicles << " " << n_customers << " " << n_depots << endl;
    while (i++ < n_depots) {
        int max_duration_per_vehicle, max_load_per_vehicle;
        data_file >> max_duration_per_vehicle >> max_load_per_vehicle;
        if(test_ONOFF) cout << max_duration_per_vehicle << " " << max_load_per_vehicle << endl;
        temp_depots.emplace(max_duration_per_vehicle, max_load_per_vehicle, 0, i);
    }

    i = 0;
    while (i++ < n_customers) {
		int index, x, y, duration, demand;
		data_file >> index >> x >> y >> duration >> demand;
		string dummy_line;
		getline(data_file, dummy_line);
        if(test_ONOFF) cout << index << " " << x << " " << y << " " << duration << " " << demand << endl;
        customers.emplace(index, x, y, duration, demand);
    }
    if(test_ONOFF) cout << "i is: " << i << endl;

    i = 0;
    set<depot>::iterator d_iter = temp_depots.begin();
    while (i++ < n_depots) {
        int x, y, dummy;
        data_file >> dummy >> x >> y ;
		string dummy_line;
		getline(data_file, dummy_line);
        if(test_ONOFF) cout << x << " " << y << endl;
        depot d = *d_iter;
        d.x = x;
        d.y = y;
        depots.insert(d);
        d_iter++;
    }
    if(test_ONOFF) cout << "i is: " << i << endl;
    
    data_file.close();
}

int read_best_fitness_solution(const string filename) {
	ifstream data_file;
	data_file.open(filename);

	if (!data_file) {
		cout << "Unable to open file!";
		exit(1);
	}

	int i = 0;
	int best_fitness = 0;
	data_file >> best_fitness;
	return best_fitness;
}