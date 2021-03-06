#include<algorithm>
#include<iostream>
#include"BCA.h"
#include"scheduler.h"
#include"JSSP.h"
#include"file.h"

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

void bee_colony_algorithm(Operation_manager& om, double target, bool minimize) {

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
	int retired_employees_trimmed = 0;

	std::vector<flowerpatch> flowerpatches;
	for (int i = 0; i < NUM_EMPLOYEES; i++) {
		flowerpatches.emplace_back();
		for (int j = 0; j < NUM_ONLOOKERS; j++) {	
			flowerpatches[i].bees.emplace_back();
		}
	}

	for (int i = 0; i < om.get_n_jobs(); i++) {
		for (int j = 0; j < om.get_n_machines(); j++) {
			flowerpatches[0].bees[0].tasks.push_back(i);
		}
	}
	for (int i = 1; i < NUM_EMPLOYEES; i++) {
		flowerpatches[i].bees[0] = flowerpatches[0].bees[0];
		std::random_shuffle(flowerpatches[i].bees[0].tasks.begin(), flowerpatches[i].bees[0].tasks.end());
	}
	std::random_shuffle(flowerpatches[0].bees[0].tasks.begin(), flowerpatches[0].bees[0].tasks.end());
	
	// Search algorithm
	int iteration = 0;
	int percent_20 = 0;
	int percent_30 = 0;
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
				mutate_search(om, flowerpatches[i].bees[j].tasks, sign);
				test_JSSP_sol(flowerpatches[i].bees[j].tasks, om.get_n_jobs(), om.get_n_machines());
			}
		}

		// Swap best employed with its best owned onlooker
		for (int patch_index = 0; patch_index < NUM_EMPLOYEES; patch_index++) {
			for (int bee_index = 0; bee_index < flowerpatches[patch_index].num_bees; bee_index++) {
				flowerpatches[patch_index].bees[bee_index].fitness = sign*develop_makespan(om, flowerpatches[patch_index].bees[bee_index].tasks);
			}
			std::vector<bee>::iterator begin = flowerpatches[patch_index].bees.begin();
			std::vector<bee>::iterator end = begin + flowerpatches[patch_index].num_bees;
			std::sort(begin, end, bee_fitness_comparator);
		}

		// ONLY IF NEEDED: If stagnation, store achieved result of employed bee, and turn into scout bee
		// If too similar, turn least fitness patch into scout bee (NOT IMPLEMENTED, but would probably be wise)
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
			if (flowerpatches[i].steps_since_improvement >= N_STEPS_BEFORE_RETIREMENT) {
				retired_employees.push_back(flowerpatches[i].bees[0]);
				if (retired_employees.size() >= 50) {
					std::sort(retired_employees.begin(), retired_employees.end(), bee_fitness_comparator);
					retired_employees.erase(retired_employees.begin() + 10);
					retired_employees_trimmed++;
					if (retired_employees_trimmed % 100 == 0) {
						std::cout << "Trimmed " << retired_employees_trimmed << " retired employee bees." << std::endl;
					}
				}
				std::random_shuffle(flowerpatches[i].bees[0].tasks.begin(), flowerpatches[i].bees[0].tasks.end());
				flowerpatches[i].steps_since_improvement = 0;
			}
		}

		// Put the employed bee and its corresponding flower patch in the correct priority bracket
		std::sort(flowerpatches.begin(), flowerpatches.end(), flowerpatch_fitness_comparator);
		//std::cout << "Best fitness: " << calc_makespan(om, flowerpatches[0].bees[0]) << std::endl;
		if (retired_employees.size() > 0) {
			std::vector<int> best_sol = flowerpatches[0].bees[0].fitness > retired_employees[0].fitness ? flowerpatches[0].bees[0].tasks : retired_employees[0].tasks;
			double best = develop_makespan(om, best_sol);
			//if (iteration % 100) std::cout << "Best fitness achieved: " << sign * best << std::endl;
			//if (target > 0 && sign*best < target*1.1) break;
			//else if (target == 0 && retired_employees_trimmed > 200) break;

			if (target*1.3 > best && percent_30 == 0) {
				std::cout << "within 30% " << best << std::endl;
				write_file("plot\\bee_sol.txt", om);
				percent_30 = 1;
			}
			else if (target*1.2 > best && percent_20 == 0) {
				std::cout << "within 20% " << best << std::endl;
				write_file("plot\\bee_sol.txt", om);
				percent_20 = 1;
			}
			else if (percent_20 == 1 && iteration % 100 == 0) {
				std::cout <<"it: " << iteration << " best: " << best << std::endl;
			}
			else if (target*1.1 > best) {
				std::cout << "on target within 10% " << best << std::endl;
				write_file("plot\\bee_sol.txt", om);
				break;
			}
		}
		iteration += 1;
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
	//calc_makespan(om, retired_employees[0].tasks);
	//om.print_operation_sequence();
}

bool bee_fitness_comparator(const bee &bee_A, const bee &bee_B) {
	return bee_A.fitness > bee_B.fitness;
}

bool flowerpatch_fitness_comparator(const flowerpatch &patch_A, const flowerpatch &patch_B) {
	return patch_A.bees[0].fitness > patch_B.bees[0].fitness;
}

void print_bee(const std::vector<int> &bee) {
	for (int i = 0; i < bee.size(); i++) {
		std::cout << bee[i] << ", ";
	}
	std::cout << std::endl;
}