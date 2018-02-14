#include <iostream>
#include "mVRP.h"
#include <ctime> 
#include <string>


int main(int num_args, char** arg_strings) {
	time_t now;
	time(&now);
	srand((unsigned int)now);

	//p01 opt, 4 depots, 50, 4 vehicles
	/*int n_individuals = 150;
	double parent_percentage = 1;
	double survivor_elitism_percentage = 0.1;
	double parent_elitism_percentage = 0.1;
	double intra_mutation_rate = 0.3;
	double inter_mutation_rate = 0.4;
	double recombination_rate = 1;
	int mutation_decay_rate = 70;
	int inter_depot_mutation_era = 10;
	double include_neighbours_inter_depot_perc = 0.1;
	double depot_availability_bound = 1.5;
	int n_generations = 3000;*/

	//p03 5%, 5 depots, 75 customers, 3 vehicles 
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

	//p10, 30% within 2000 generations, 4 depots, 249 customers, 8 vehicles
	/*int n_individuals = 60;
	double parent_percentage = 0.8; // Should be 1?
	double survivor_elitism_percentage = 0.1;
	double parent_elitism_percentage = 0.1;
	double intra_mutation_rate = 0.4;
	double inter_mutation_rate = 0.3;
	double recombination_rate = 1; // Should be 0.8?
	int mutation_decay_rate = 800;
	int inter_depot_mutation_era = 10;
	double include_neighbours_inter_depot_perc = 0;
	double depot_availability_bound = 1.5;
	int n_generations = 2000*/

	//p10 take 2
	/*int n_individuals = 30;
	double parent_percentage = 1;
	double survivor_elitism_percentage = 0.1;
	double parent_elitism_percentage = 0.1;
	double intra_mutation_rate = 0.3;
	double inter_mutation_rate = 0.25;
	double recombination_rate = 1;
	int mutation_decay_rate = 800;
	int inter_depot_mutation_era = 10;
	double include_neighbours_inter_depot_perc = 0.1;
	double depot_availability_bound = 0.8;
	int n_generations = 3000;*/

	//p04 might make 5%, seems to be better
	//2 depots 8 vehicles 100 customers
	/*int n_individuals = 150;
	double parent_percentage = 1;
	double survivor_elitism_percentage = 0.1;
	double parent_elitism_percentage = 0.1;
	double intra_mutation_rate = 0.3;
	double inter_mutation_rate = 0.4;
	double recombination_rate = 1;
	int mutation_decay_rate = 800;
	int inter_depot_mutation_era = 10;
	double include_neighbours_inter_depot_perc = 0;
	double depot_availability_bound = 2;
	int n_generations = 3000;*/

	//p05 2 depots, 100 customers, 5 vehicles, should have same logic as p04
	/*int n_individuals = 150;
	double parent_percentage = 1;
	double survivor_elitism_percentage = 0.1;
	double parent_elitism_percentage = 0.1;
	double intra_mutation_rate = 0.3;
	double inter_mutation_rate = 0.4;
	double recombination_rate = 1;
	int mutation_decay_rate = 800;
	int inter_depot_mutation_era = 10;
	double include_neighbours_inter_depot_perc = 0.1;
	double depot_availability_bound = 0.8;
	int n_generations = 3000;*/

	// p11 not close enough, 5 depots, 6 vehicles 249 customers
	int n_individuals = 60;
	double parent_percentage = 1;
	double survivor_elitism_percentage = 0.05;
	double parent_elitism_percentage = 0.01;
	double intra_mutation_rate = 0.8;
	double inter_mutation_rate = 0.4;
	double recombination_rate = 0.9;
	int mutation_decay_rate = 1600;
	int inter_depot_mutation_era = 5;
	double include_neighbours_inter_depot_perc = 0;
	double depot_availability_bound = 0.8;
	int n_generations = 3000;
	
	// p20
	/*int n_individuals = 150;
	double parent_percentage = 1;
	double survivor_elitism_percentage = 1;
	double parent_elitism_percentage = 1;
	double intra_mutation_rate = 0.3;
	double inter_mutation_rate = 0.4;
	double recombination_rate = 1;
	int mutation_decay_rate = 800;
	int inter_depot_mutation_era = 10;
	double include_neighbours_inter_depot_perc = 0.1;
	double depot_availability_bound = 0.8;
	int n_generations = 3000;*/



	std::string data_filename = "..\\..\\testing_data\\data_files\\p05";
	std::string solution_filename = "..\\..\\testing_data\\solution_files\\p05.res";
	GA_mVRP(n_individuals, parent_percentage, survivor_elitism_percentage, parent_elitism_percentage, intra_mutation_rate, inter_mutation_rate, recombination_rate, mutation_decay_rate, inter_depot_mutation_era, include_neighbours_inter_depot_perc, depot_availability_bound, n_generations, data_filename, solution_filename);

	system("pause");
	return 0;
}