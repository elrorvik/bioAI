#pragma once
#include<vector>

const int invalid_city = -1;
enum selection_on { offspring_selection, parent_selection, population_selection };


struct customer {
    int index = 0;
    int x = 0;
    int y = 0;
    int duration = 0;
    int demand = 0;
	std::vector<int> depot_available;

	customer(int index, int x, int y, int duration, int demand) : index(index), x(x), y(y), duration(duration), demand(demand) {};
	customer() :index(invalid_city), x(0), y(0), duration(0), demand(0) {};
	//void operator=(const customer &other);
};

bool operator<(const customer &right, const customer &left);

struct depot{
    int max_duration_per_vehicle;
    int max_load_per_vehicle;
    int x;
    int y;

    depot(int max_duration_per_vehicle, int max_load_per_vehicle, int x, int y)
        : max_duration_per_vehicle(max_duration_per_vehicle), max_load_per_vehicle(max_load_per_vehicle), x(x), y(y) {}

	depot() {}
};

bool operator<(const depot &right, const depot &left);


void GA_mVRP(int n_individuals, double parent_percentage, double survivor_elitism_percentage, double parent_elitism_percentage, double intra_mutation_rate, double inter_mutation_rate, double recombination_rate, std::string data_filename, std::string solution_filename);

double exponential_decay(int index, int time_constant);