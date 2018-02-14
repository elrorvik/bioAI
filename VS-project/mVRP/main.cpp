#include <iostream>
#include "mVRP.h"
#include <ctime> 
#include <string>


int main(int num_args, char** arg_strings) {
	time_t now;
	time(&now);
	srand((unsigned int)now);

	//p03 5%
	/*int n_individuals = 150;
	double parent_percentage = 1;
	double survivor_elitism_percentage = 0.1;
	double parent_elitism_percentage = 0.01;
	double intra_mutation_rate = 0.25;
	double inter_mutation_rate = 0.4;
	double recombination_rate = 0.60;
	int mutation_decay_rate = 40;
	int inter_depot_mutation_era = 5;
	double include_neighbours_inter_depot = 0;
	double depot_availability_bound = 1.5;*/

	//TODO: Make depot_availability_bound work
	//TODO: Remove validate individual

	//p10, 30% within 2000 generations
	/*int n_individuals = 60;
	double parent_percentage = 0.8; // Turn down?
	double survivor_elitism_percentage = 0.1;
	double parent_elitism_percentage = 0.1; // Turn up?
	double intra_mutation_rate = 0.4;
	double inter_mutation_rate = 0.3;
	double recombination_rate = 1;
	int mutation_decay_rate = 800;
	int inter_depot_mutation_era = 10;
	double include_neighbours_inter_depot_perc = 0;
	double depot_availability_bound = 1.5;*/

	//p10 take 2
	int n_individuals = 200;
	double parent_percentage = 1;
	double survivor_elitism_percentage = 0.1;
	double parent_elitism_percentage = 0.1;
	double intra_mutation_rate = 0.4;
	double inter_mutation_rate = 0.3;
	double recombination_rate = 0.6;
	int mutation_decay_rate = 800;
	int inter_depot_mutation_era = 10;
	double include_neighbours_inter_depot_perc = 0; // NEXT, try this
	double depot_availability_bound = 1.5;

	std::string data_filename = "..\\..\\testing_data\\data_files\\p10";
	std::string solution_filename = "..\\..\\testing_data\\solution_files\\p10.res";
	GA_mVRP(n_individuals, parent_percentage, survivor_elitism_percentage, parent_elitism_percentage, intra_mutation_rate, inter_mutation_rate, recombination_rate, mutation_decay_rate, inter_depot_mutation_era, include_neighbours_inter_depot_perc, depot_availability_bound, data_filename, solution_filename);

	system("pause");
	return 0;
}