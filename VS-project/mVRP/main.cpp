#include <iostream>
#include "mVRP.h"
#include "visualization.h"
#include <ctime> 
#include <string>


int main(int num_args, char** arg_strings) {
	time_t now;
	time(&now);
	srand((unsigned int)now);

	int n_individuals = 100;
	double parent_percentage = 0.02;
	double survivor_elitism_percentage = 1;
	double parent_elitism_percentage = 1;
	std::string filename = "..\\..\\testing_data\\data_files\\p03";
	GA_mVRP(n_individuals, parent_percentage, survivor_elitism_percentage, parent_elitism_percentage, filename);

	system("pause");
	return 0;
}