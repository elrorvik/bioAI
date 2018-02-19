#include"file.h"
#include <iostream>
#include<fstream>

bool test_ONOFF = false;

void read_data(const string filename, set<customer> &customers, vector<depot> &depots, int &n_vehicles, int &n_customers, int &n_depots) {
    ifstream data_file;
    data_file.open(filename);

    if (!data_file) {
        cout << "Unable to open file!";
        exit(1);
    }

    int i = -1;
    vector<depot> temp_depots;
    data_file >> n_vehicles >> n_customers >> n_depots;
    if(test_ONOFF) cout << n_vehicles << " " << n_customers << " " << n_depots << endl;
    while (++i < n_depots) {
        int max_duration_per_vehicle, max_load_per_vehicle;
        data_file >> max_duration_per_vehicle >> max_load_per_vehicle;
        if(test_ONOFF) cout << max_duration_per_vehicle << " " << max_load_per_vehicle << endl;
		depot d(max_duration_per_vehicle, max_load_per_vehicle, 0, i);
        temp_depots.push_back(d);
    }

    i = -1;
    while (++i < n_customers) {
		int index, x, y, duration, demand;
		data_file >> index >> x >> y >> duration >> demand;
		string dummy_line;
		getline(data_file, dummy_line);
        if(test_ONOFF) cout << index << " " << x << " " << y << " " << duration << " " << demand << endl;
        customers.emplace(index, x, y, duration, demand);
    }
    if(test_ONOFF) cout << "i is: " << i << endl;

    i = -1;


	depots = temp_depots;
    while (++i < n_depots) {
		/*std::cout << i << std::endl;
		std::cin.get();*/
        int dummy, x, y;
        data_file >> dummy >> x >> y ;
		string dummy_line;
		getline(data_file, dummy_line);
        //cout << x << " " << y << endl;
        depots[i].x = x;
        depots[i].y = y;
		//std::cin.get();
    }
    if(test_ONOFF) cout << "i is: " << i << endl;

	/*for (auto it = depots.begin(); it != depots.end(); it++) {
		std::cout << it->x << " x, " << it->y << " y." << std::endl;
	}*/
    
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