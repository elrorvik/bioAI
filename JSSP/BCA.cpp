#include<algorithm>
#include<iostream>
#include"BCA.h"
#include"scheduler.h"

struct bee {
	std::vector<int> tasks;
	double fitness;
};

struct flowerpatch {
	std::vector<bee> bees;
	int num_bees = 0;
	double prev_best_fitness = DBL_MAX;
	int steps_since_improvement = 0;
	// Want to sort this with the best fitness first this is always the employee bee
	// Every step, we set all other bees of the patch to be equal to the best bee, then change every other bee for onlooker bee search
	// How to assign onlooker bees?
};

// Local functions, should not be visible to other modules
bool bee_fitness_comparator(const bee &bee_A, const bee &bee_B);
bool flowerpatch_fitness_comparator(const flowerpatch &patch_A, const flowerpatch &patch_B);
void mutate_bee(bee &bee);
void print_bee(const bee &bee);

void bee_colony_algorithm(Operation_manager& om, bool minimize) {

	// Note:
	// Three things that can be improved:
	// Mutations - could be more of them or they could be done differently, current solution is very arbitrary
	// How the BCA decides when it is finished could be better, current solution is very arbitrary
	// More flowerpatches with the lowest max fitness (perhaps the two worst flowerpatches) could be randomly reset, i.e used for scouting instead. If not all of those in the lower priority bracket.

	// Initialize flowerpatches and bees
	//std::cout << "Initialize!" << std::endl << std::endl;

	double sign = 1;
	if (minimize) sign = -1;

	std::vector<bee> retired_employees;

	std::vector<flowerpatch> flowerpatches;
	for (int i = 0; i < NUM_EMPLOYEES; i++) {
		flowerpatches.emplace_back();
		for (int j = 0; j < NUM_ONLOOKERS; j++) {	
			flowerpatches[i].bees.emplace_back();
		}
	}

	for (int i = 0; i < om.get_n_jobs(); i++) {
		for (int j = 0; j < om.get_op_size(i); j++) {
			flowerpatches[0].bees[0].tasks.push_back(i);
		}
	}
	for (int i = 1; i < NUM_EMPLOYEES; i++) {
		flowerpatches[i].bees[0] = flowerpatches[0].bees[0];
		std::random_shuffle(flowerpatches[i].bees[0].tasks.begin(), flowerpatches[i].bees[0].tasks.end());
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
			test_JSSP_sol(flowerpatches[i].bees[0].tasks, om.get_n_jobs(), om.get_n_machines());
			for (int j = 1; j < flowerpatches[i].num_bees; j++) {
				flowerpatches[i].bees[j] = flowerpatches[i].bees[0];
				mutate_bee(flowerpatches[i].bees[j]);
				test_JSSP_sol(flowerpatches[i].bees[j].tasks, om.get_n_jobs(), om.get_n_machines());
			}
		}

		// Swap best employed with its best owned onlooker
		for (int patch_index = 0; patch_index < NUM_EMPLOYEES; patch_index++) {
			for (int bee_index = 0; bee_index < flowerpatches[patch_index].num_bees; bee_index++) {
				flowerpatches[patch_index].bees[bee_index].fitness = sign*calc_makespan(om, flowerpatches[patch_index].bees[bee_index].tasks);
			}
			std::vector<bee>::iterator begin = flowerpatches[patch_index].bees.begin();
			std::vector<bee>::iterator end = begin + flowerpatches[patch_index].num_bees;
			std::sort(begin, end, bee_fitness_comparator);
		}

		// ONLY IF NEEDED: If stagnation, store achieved result of employed bee, and turn into scout bee
		// If too similar, turn least fitness patch into scout bee
		for (int i = 0; i < NUM_EMPLOYEES; i++) {
			if (i >= NUM_EMPLOYEES - 2) {
				std::random_shuffle(flowerpatches[i].bees[0].tasks.begin(), flowerpatches[i].bees[0].tasks.end());
				flowerpatches[i].steps_since_improvement = 0;
				continue;
			}

			//for (int j = i + 1; j < NUM_EMPLOYEES; j++) { // Too similar to eachother
			//	if (bee_similar(flowerpatches[i].bees[0], flowerpatches[j].bees[0])) {
			//		if (calc_makespan(om, flowerpatches[i].bees[0]) < calc_makespan(om, flowerpatches[j].bees[0])) std::random_shuffle(flowerpatches[j].bees[0].begin(), flowerpatches[j].bees[0].end());
			//		else std::random_shuffle(flowerpatches[i].bees[0].begin(), flowerpatches[i].bees[0].end());
			//	}
			//}
			//for (int j = 0; j < retired_employees.size(); j++) { // Too similar to a retired bee
			//	if (bee_similar(flowerpatches[i].bees[0], retired_employees[j])) flowerpatches[i].bees[0].begin(), flowerpatches[i].bees[0].end();
			//}

			if (flowerpatches[i].prev_best_fitness >= flowerpatches[i].bees[0].fitness) flowerpatches[i].steps_since_improvement += 1;
			else flowerpatches[i].steps_since_improvement = 0;
			if (flowerpatches[i].steps_since_improvement >= 5) {
				retired_employees.push_back(flowerpatches[i].bees[0]);
				std::random_shuffle(flowerpatches[i].bees[0].tasks.begin(), flowerpatches[i].bees[0].tasks.end());
				flowerpatches[i].steps_since_improvement = 0;
			}
		}

		// Put the employed bee and its corresponding flower patch in the correct priority bracket
		std::sort(flowerpatches.begin(), flowerpatches.end(), flowerpatch_fitness_comparator);
		//std::cout << "Best fitness: " << calc_makespan(om, flowerpatches[0].bees[0]) << std::endl;
		if (retired_employees.size() > 50) break;
	}
	retired_employees.push_back(flowerpatches[0].bees[0]);
	std::sort(retired_employees.begin(), retired_employees.end(), bee_fitness_comparator);
	// DEBUG:
	//std::cout << std::endl << std::endl << "Best fitness: " << max_fitness << std::endl;
	//for (int i = 0; i < retired_employees.size(); i++) {
	//	std::cout << "Retired fitness: " << calc_makespan(om, retired_employees[i]) << std::endl;
	//}
	//for (int i = 0; i < flowerpatches[0].num_bees; i++) {
	//	std::cout << "Flowerpatch 0 fitness: " << calc_makespan(om, flowerpatches[0].bees[i]) << std::endl;
	//}
	//for (int i = 0; i < flowerpatches[1].num_bees; i++) {
	//	std::cout << "Flowerpatch 1 fitness: " << calc_makespan(om, flowerpatches[1].bees[i]) << std::endl;
	//}
	// :DEBUG

	std::cout << "Bee algorithm terminated with best fitness: " << sign*retired_employees[0].fitness << " with solution:" << std::endl;
	calc_makespan(om, retired_employees[0].tasks);
	om.print_operation_sequence();
}

bool bee_fitness_comparator(const bee &bee_A, const bee &bee_B) {
	return bee_A.fitness > bee_B.fitness;
}

bool flowerpatch_fitness_comparator(const flowerpatch &patch_A, const flowerpatch &patch_B) {
	return patch_A.bees[0].fitness > patch_B.bees[0].fitness;
}

void mutate_bee(bee &bee) {
	int num_mutations = (rand() % 5) + 1;
	int mutation_index = 0;
	while (mutation_index < num_mutations) {
		double mutation_outcome = (rand() % 10) / 10.0;
		if (mutation_outcome < 0.5) { // Swap
			//std::cout << "Swapping" << std::endl;
			//print_bee(bee);
			int pos_A = rand() % bee.tasks.size();
			int pos_B;
			do {
				pos_B = rand() % bee.tasks.size();
			} while (pos_A == pos_B);
			//std::cout << "Pos A: " << pos_A << ", with Pos B: " << pos_B << std::endl;
			int temp = bee.tasks[pos_A];
			bee.tasks[pos_A] = bee.tasks[pos_B];
			bee.tasks[pos_B] = temp;
			//print_bee(bee);
		}
		else { // Invert
			//std::cout << "Inverting!" << std::endl;
			int pos_A = rand() % bee.tasks.size();
			int pos_B;
			do {
				int sign = rand() % 2;
				int diff = (rand() % 2) + 2;
				pos_B = pos_A + diff - 2 * diff*sign;
			} while (pos_B >= bee.tasks.size() || pos_B < 0);
			if (pos_A > pos_B) {
				int temp = pos_A;
				pos_A = pos_B;
				pos_B = temp;
			}
			//std::cout << "Pos A: " << pos_A << ", to Pos B: " << pos_B << std::endl;
			std::vector<int>::iterator bee_it_A = bee.tasks.begin() + pos_A;
			std::vector<int>::iterator bee_it_B = bee.tasks.begin() + pos_B;
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