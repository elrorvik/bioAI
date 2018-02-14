#include <iostream>
#include "mVRP.h"
#include <ctime> 
#include <string>


int main(int num_args, char** arg_strings) {
	time_t now;
	time(&now);
	srand((unsigned int)now);

	int n_individuals = 100;
	double parent_percentage = 1;
	double survivor_elitism_percentage = 0.1;
	double parent_elitism_percentage = 0.01;
	double intra_mutation_rate = 0.25;
	double inter_mutation_rate = 0.4;
	double recombination_rate = 0.60;
	std::string filename = "..\\..\\testing_data\\data_files\\p03";
	GA_mVRP(n_individuals, parent_percentage, survivor_elitism_percentage, parent_elitism_percentage, intra_mutation_rate, inter_mutation_rate, recombination_rate, filename);

	system("pause");
	return 0;
}