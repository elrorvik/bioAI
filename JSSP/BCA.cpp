#include<algorithm>
#include<iostream>
#include"BCA.h"

struct flowerpatch {
	std::vector<std::vector<int>> bees;
	int num_bees = 0;
	double prev_best_fitness = DBL_MAX;
	int steps_since_improvement = 0;
	// Want to sort this with the best fitness first this is always the employee bee
	// Every step, we set all other bees of the patch to be equal to the best bee, then change every other bee for onlooker bee search
	// How to assign onlooker bees?
};

bool bee_fitness_comparator(const std::vector<int> &bee_A, const std::vector<int> &bee_B);
bool flowerpatch_fitness_comparator(const flowerpatch &patch_A, const flowerpatch &patch_B);
double fitness(const std::vector<int> &bee);
void mutate_bee(std::vector<int> &bee);
void print_bee(const std::vector<int> &bee);

void bee_colony_algorithm(Operation_manager &om) {

	// Note:
	// Three things that can be improved:
	// Mutations - could be more of them or they could be done differently, current solution is very arbitrary
	// How the BCA decides when it is finished could be better, current solution is very arbitrary
	// More flowerpatches with the lowest max fitness (perhaps the two worst flowerpatches) could be randomly reset, i.e used for scouting instead. If not all of those in the lower priority bracket.

	// Initialize flowerpatches and bees
	//std::cout << "Initialize!" << std::endl << std::endl;
	
	std::vector<std::vector<int>> retired_employees;

	std::vector<flowerpatch> flowerpatches;
	for (int i = 0; i < NUM_EMPLOYEES; i++) {
		flowerpatches.emplace_back();
		for (int j = 0; j < NUM_ONLOOKERS; j++) {
			flowerpatches[i].bees.emplace_back();
		}
	}

	for (int i = 0; i < om.get_n_jobs(); i++) {
		for (int j = 0; j < om.get_op_size(i); j++) {
			flowerpatches[0].bees[0].push_back(i);
		}
	}
	for (int i = 1; i < NUM_EMPLOYEES; i++) {
		flowerpatches[i].bees[0] = flowerpatches[0].bees[0];
		std::random_shuffle(flowerpatches[i].bees[0].begin(), flowerpatches[i].bees[0].end());
	}	
	
	// Search algorithm
	while (1) {
		//std::cout << "Step!" << std::endl << std::endl;
		// Recruit onlooker bees, higher priority bracket employees gets more
		int num_onlookers_recruited = 0;
		for (int i = 0; i < NUM_EMPLOYEES; i++) {
			int cap;
			if (i < NUM_PRIORITY_EMPLOYEES) cap = NUM_ONLOOKERS_PER_PRIORITY_EMPLOYEES;
			else cap = NUM_ONLOOKERS_PER_NONPRIORITY_EMPLOYEES;

			int num_onlooker_bees = rand() % cap;
			flowerpatches[i].num_bees = num_onlooker_bees + 1; // +1 for the employer bee as well
			num_onlookers_recruited += num_onlooker_bees;
		}
		{
			int patch_index = 0;
			while (num_onlookers_recruited < NUM_ONLOOKERS) {
				flowerpatches[patch_index++].num_bees += 1;
				num_onlookers_recruited += 1;
				if (patch_index >= NUM_EMPLOYEES) patch_index = 0;
			}
		}

		// DEBUG:
		//std::cout << "Num bees per patch:" << std::endl;
		//for (int i = 0; i < NUM_EMPLOYEES; i++) {
		//	std::cout << flowerpatches[i].num_bees << std::endl;
		//}
		//std::cout << std::endl;
		// :DEBUG

		// Position onlooker bees near employed bee of its patch
		for (int i = 0; i < NUM_EMPLOYEES; i++) {
			for (int j = 1; j < flowerpatches[i].num_bees; j++) {
				flowerpatches[i].bees[j] = flowerpatches[i].bees[0];
				mutate_bee(flowerpatches[i].bees[j]);
			}
		}

		// Swap best employed with its best owned onlooker
		for (int i = 0; i < NUM_EMPLOYEES; i++) {
			std::vector<std::vector<int>>::iterator begin = flowerpatches[i].bees.begin();
			std::vector<std::vector<int>>::iterator end = begin + flowerpatches[i].num_bees;
			std::sort(begin, end, bee_fitness_comparator);
		}

		// ONLY IF NEEDED: If stagnation, store achieved result of employed bee, and turn into scout bee
		// If too similar, turn least fitness patch into scout bee
		for (int i = 0; i < NUM_EMPLOYEES; i++) {
			if (i >= NUM_EMPLOYEES - 2) {
				std::random_shuffle(flowerpatches[i].bees[0].begin(), flowerpatches[i].bees[0].end());
				flowerpatches[i].steps_since_improvement = 0;
				continue;
			}

			//for (int j = i + 1; j < NUM_EMPLOYEES; j++) { // Too similar to eachother
			//	if (bee_similar(flowerpatches[i].bees[0], flowerpatches[j].bees[0])) {
			//		if (fitness(flowerpatches[i].bees[0]) < fitness(flowerpatches[j].bees[0])) std::random_shuffle(flowerpatches[j].bees[0].begin(), flowerpatches[j].bees[0].end());
			//		else std::random_shuffle(flowerpatches[i].bees[0].begin(), flowerpatches[i].bees[0].end());
			//	}
			//}
			//for (int j = 0; j < retired_employees.size(); j++) { // Too similar to a retired bee
			//	if (bee_similar(flowerpatches[i].bees[0], retired_employees[j])) flowerpatches[i].bees[0].begin(), flowerpatches[i].bees[0].end();
			//}

			if (flowerpatches[i].prev_best_fitness >= fitness(flowerpatches[i].bees[0])) flowerpatches[i].steps_since_improvement += 1;
			else flowerpatches[i].steps_since_improvement = 0;
			if (flowerpatches[i].steps_since_improvement >= 5) {
				retired_employees.push_back(flowerpatches[i].bees[0]);
				std::random_shuffle(flowerpatches[i].bees[0].begin(), flowerpatches[i].bees[0].end());
				flowerpatches[i].steps_since_improvement = 0;
			}
		}

		// Put the employed bee and its corresponding flower patch in the correct priority bracket
		std::sort(flowerpatches.begin(), flowerpatches.end(), flowerpatch_fitness_comparator);
		//std::cout << "Best fitness: " << fitness(flowerpatches[0].bees[0]) << std::endl;
		if (retired_employees.size() > 50) break;
	}
	std::sort(retired_employees.begin(), retired_employees.end(), bee_fitness_comparator);
	double max_fitness = fitness(flowerpatches[0].bees[0]) >= fitness(retired_employees[0]) ? fitness(flowerpatches[0].bees[0]) : fitness(retired_employees[0]);
	// DEBUG:
	//std::cout << std::endl << std::endl << "Best fitness: " << max_fitness << std::endl;
	//for (int i = 0; i < retired_employees.size(); i++) {
	//	std::cout << "Retired fitness: " << fitness(retired_employees[i]) << std::endl;
	//}
	//for (int i = 0; i < flowerpatches[0].num_bees; i++) {
	//	std::cout << "Flowerpatch 0 fitness: " << fitness(flowerpatches[0].bees[i]) << std::endl;
	//}
	//for (int i = 0; i < flowerpatches[1].num_bees; i++) {
	//	std::cout << "Flowerpatch 1 fitness: " << fitness(flowerpatches[1].bees[i]) << std::endl;
	//}
	// :DEBUG
}

bool bee_fitness_comparator(const std::vector<int> &bee_A, const std::vector<int> &bee_B) {
	return fitness(bee_A) > fitness(bee_B);
}

bool flowerpatch_fitness_comparator(const flowerpatch &patch_A, const flowerpatch &patch_B) {
	return fitness(patch_A.bees[0]) > fitness(patch_B.bees[0]);
}

double fitness(const std::vector<int> &bee) { // Dummy fitness function
	return bee[0] * 3.14 + bee[5] * 3.14 + bee[10] * 3.14 + bee[15] * 3.14 - bee[3] - bee[18] - bee[11];
}

void mutate_bee(std::vector<int> &bee) {
	int num_mutations = (rand() % 5) + 1;
	int mutation_index = 0;
	while (mutation_index < num_mutations) {
		double mutation_outcome = (rand() % 10) / 10.0;
		if (mutation_outcome < 0.5) { // Swap
			//std::cout << "Swapping" << std::endl;
			//print_bee(bee);
			int pos_A = rand() % bee.size();
			int pos_B;
			do {
				pos_B = rand() % bee.size();
			} while (pos_A == pos_B);
			//std::cout << "Pos A: " << pos_A << ", with Pos B: " << pos_B << std::endl;
			int temp = bee[pos_A];
			bee[pos_A] = bee[pos_B];
			bee[pos_B] = temp;
			//print_bee(bee);
		}
		else { // Invert
			//std::cout << "Inverting!" << std::endl;
			int pos_A = rand() % bee.size();
			int pos_B;
			do {
				int sign = rand() % 2;
				int diff = (rand() % 2) + 2;
				pos_B = pos_A + diff - 2 * diff*sign;
			} while (pos_B >= bee.size() || pos_B < 0);
			if (pos_A > pos_B) {
				int temp = pos_A;
				pos_A = pos_B;
				pos_B = temp;
			}
			//std::cout << "Pos A: " << pos_A << ", to Pos B: " << pos_B << std::endl;
			std::vector<int>::iterator bee_it_A = bee.begin() + pos_A;
			std::vector<int>::iterator bee_it_B = bee.begin() + pos_B;
			//print_bee(bee);
			std::reverse(bee_it_A, bee_it_B);
			//print_bee(bee);
		}
		mutation_index += 1;
	}
}

void print_bee(const std::vector<int> &bee) {
	for (int i = 0; i < bee.size(); i++) {
		std::cout << bee[i] << ", ";
	}
	std::cout << std::endl;
}

//bool bee_similar(std::vector<int> bee_A, std::vector<int> bee_B) { // ONLY IF NEEDED!
	// If they have L amount of jobs in similar order, or K*P amount of jobs in P similar order K times or something, they are too similar
//}

