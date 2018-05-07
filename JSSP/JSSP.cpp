#include<iostream>
#include<string>
#include"JSSP.h"
#include"operation_manager.h"
#include"scheduler.h"

void swap(std::vector<int> &sol, int start, int end);
void invert(std::vector<int> &sol, int start, int end);

void mutate(std::vector<int> &sol) {
	int num_mutations = (rand() % 5) + 1;
	int mutation_index = 0;
	while (mutation_index < num_mutations) {
		double mutation_outcome = (rand() % 10) / 10.0;
		if (mutation_outcome < 0.5) {
			swap(sol, 0, sol.size());
		}
		else {
			invert(sol, 0, sol.size());
		}
		mutation_index += 1;
	}
}

bool mutate_search(Operation_manager& om, std::vector<int> &sol, int sign) {
	bool improved = false;
	int num_mutations = (rand() % 5) + 1;
	int mutation_index = 0;

	std::vector<int> best_sol = sol;
	double best_fitness = sign * calc_makespan(om, sol);

	while (mutation_index < num_mutations) {
		double mutation_outcome = (rand() % 10) / 10.0;
		if (mutation_outcome < 0.5) {
			swap(sol, 0, sol.size());
		}
		else {
			invert(sol, 0, sol.size());
		}
		double new_fitness = sign * calc_makespan(om, sol);
		if (best_fitness < new_fitness) {
			best_fitness = new_fitness;
			mutation_index += 1;
			improved = true;
		}
		else {
			sol = best_sol;
			if (rand() % 10 > 2) mutation_index += 1;
		}
	}
	return improved;
}

void mutate_search_greedy(Operation_manager& om, std::vector<int> &sol, int sign) {
	int length = (rand() % 8) + 4;
	int start = rand() % (sol.size() - length);
	search_greedy(om, sol, sign, start, start + length);
}

void search_greedy(Operation_manager& om, std::vector<int> &sol, int sign, int start, int end) {
	int num_mutations = (rand() % 5) + 1;
	int mutation_index = 0;

	std::vector<int> best_sol = sol;
	double best_fitness = sign * calc_makespan(om, sol);

	int iteration = 0;
	while(mutation_index < num_mutations) {
		double mutation_outcome = (rand() % 10) / 10.0;
		if (mutation_outcome < 0.5) {
			swap(sol, start, end);
		}
		else {
			invert(sol, start, end);
		}
		if (iteration % 3 == 0) {
			double new_fitness = sign * calc_makespan(om, sol);
			if (best_fitness < new_fitness) {
				best_fitness = new_fitness;
				mutation_index += 1;
			}
			else {
				sol = best_sol;
				if (rand() % 10 > 4) mutation_index += 1;
			}
		}
	}
}

void swap(std::vector<int> &sol, int start, int end) {
	int length = end - start;
	int pos_A = (rand() % length) + start;
	int pos_B;
	do {
		pos_B = (rand() % length) + start;
	} while (pos_A == pos_B);
	int temp = sol[pos_A];
	sol[pos_A] = sol[pos_B];
	sol[pos_B] = temp;
}

void invert(std::vector<int> &sol, int start, int end) {
	int length = end - start;
	int pos_A = (rand() % length) + start;
	int pos_B;
	do {
		int sign = rand() % 2;
		int diff = (rand() % 2) + 2;
		pos_B = pos_A + diff - 2 * diff*sign;
	} while (pos_B >= end || pos_B < start);
	if (pos_A > pos_B) {
		int temp = pos_A;
		pos_A = pos_B;
		pos_B = temp;
	}
	std::vector<int>::iterator sol_it_A = sol.begin() + pos_A;
	std::vector<int>::iterator sol_it_B = sol.begin() + pos_B;
	std::reverse(sol_it_A, sol_it_B);
}

