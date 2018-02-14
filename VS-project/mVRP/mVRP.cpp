#include <iostream>
#include <set>
#include "mVRP.h"
#include "population.h"
#include "file.h"
#include <string>

/*void customer::operator=(const customer &other) {
	this->index = other.index;
	this->x = other.x;
	this->y = other.y;
	this->duration = other.duration;
	this->demand = other.demand;
}*/

bool operator<(const depot &right, const depot &left) {
	return (right.x + right.y < left.x + left.y);
}

bool operator<(const customer &right, const customer &left) {
	return (right.index < left.index);
}

void GA_mVRP(int n_individuals, double parent_percentage, double survivor_elitism_percentage, double parent_elitism_percentage, double intra_mutation_rate, double inter_mutation_rate, double recombination_rate, int mutation_decay_rate, int inter_depot_mutation_era, double include_neighbours_inter_depot_perc, double depot_availability_bound, std::string data_filename, std::string solution_filename) {
	std::set<customer> customers;
	std::set<depot> depots;
	int n_vehicles;
	int n_customers;
	int n_depots;
	int n_parents = static_cast<int>(parent_percentage*n_individuals);
	int n_parents_elitism = static_cast<int>(parent_elitism_percentage*n_parents);
	int n_parents_sus = n_parents - n_parents_elitism;
	int n_survivor_elitism = static_cast<int>(survivor_elitism_percentage*n_individuals);
	int n_survivor_sus = n_individuals - n_survivor_elitism;

	read_data(data_filename, customers, depots, n_vehicles, n_customers, n_depots);
	int fitness_solution = read_best_fitness_solution(solution_filename);
	double within05percent = fitness_solution * 1.05;
	double within10percent = fitness_solution * 1.10;
	double within20percent = fitness_solution * 1.20;
	double within30percent = fitness_solution * 1.30;

	/*std::cout << "p01 optimal value: " << 576.87 << std::endl;
	std::cout << "5% target: " << 576.87 + 576.87*0.05 << std::endl;
	std::cout << "10% target: " << 576.87 + 576.87*0.1 << std::endl;

	std::cin.get();*/

	// psedocode for whole algorithm:

	Population population(n_vehicles, n_customers, n_depots, n_individuals, n_parents, customers, depots);
	population.initialize_population_k_mean();
	population.fitness_population_initalization();
	population.initialize_depot_customer_availability();

	int improvement_in_fitness = 2000;
	double best_fitness = DBL_MAX;
	int generation = 0;
	int n_generations = 2000;
	double second_generation_best_fitness = 0;
	int n_generations_without_improvement = 0;
	
	while (improvement_in_fitness > 0 && generation < n_generations) {
		// apply recombination on parent_index
		std::set<int> parent_index;
		population.selection_ellitisme(n_parents_elitism, parent_index, parent_selection);
		population.selection_SUS(n_parents_sus, parent_index, parent_selection);
		population.insert_greedy_recombination_in_population_random_pairing(&parent_index, recombination_rate);

		/*std::set<int> parent_index_2;
		int n_elitism_2 = 0;
		if (rand() % 2 == 0) n_elitism_2 = 1;
		if (n_parents < n_individuals) population.selection_SUS(n_parents_greedier, parent_index_2, parent_selection);
		population.selection_ellitisme(n_elitism_2, parent_index_2, parent_selection);

		population.insert_greedier_recombination_in_population_random_pairing(&parent_index_2);*/


		// apply mutation on offspring by percentages
		int era = 5;
		//if (n_generations_without_improvement >= 10) era = 2;
		double inverse_intra_vehicle_perc = 0.33*intra_mutation_rate;
		double swap_intra_depot_perc = 0.33*intra_mutation_rate;
		double customer_intra_depot_optimally_perc = 0.33*intra_mutation_rate;
		inverse_intra_vehicle_perc += inverse_intra_vehicle_perc * exponential_decay(generation, mutation_decay_rate) - inverse_intra_vehicle_perc * 0.33;
		swap_intra_depot_perc += swap_intra_depot_perc * exponential_decay(generation, mutation_decay_rate) - swap_intra_depot_perc * 0.33;
		customer_intra_depot_optimally_perc += customer_intra_depot_optimally_perc * exponential_decay(generation, mutation_decay_rate) - customer_intra_depot_optimally_perc * 0.33;
		double insert_inter_depot_perc = 1 * inter_mutation_rate;
		insert_inter_depot_perc = insert_inter_depot_perc * exponential_decay(generation, mutation_decay_rate) - insert_inter_depot_perc * 0.1;
		//if (n_generations_without_improvement >= 10) 0.8;

		int mutation_method = 0;
		if (generation % inter_depot_mutation_era != 0) mutation_method = population.insert_intra_mutation_in_offspring(inverse_intra_vehicle_perc, swap_intra_depot_perc, customer_intra_depot_optimally_perc);
		else mutation_method = population.insert_inter_mutation_in_offspring(insert_inter_depot_perc, include_neighbours_inter_depot_perc);

		// select survivors
		std::set<int> selected_index;
		population.selection_ellitisme(n_survivor_elitism, selected_index, population_selection);
		population.selection_SUS(n_survivor_sus, selected_index, population_selection);

		// remove non-survivors
		population.remove_not_selected_members(&selected_index);

		// check fitness progression
		double current_best_fitness = population.get_best_fitness();
		if (current_best_fitness >= best_fitness) {
			improvement_in_fitness--;
			n_generations_without_improvement++;
		}
		else {
			best_fitness = current_best_fitness;
			improvement_in_fitness = 4000;
			n_generations_without_improvement = 0;
			//population.write_result_to_file("..\\..\\solution_after_improvement.txt");
		}
		if (generation == 0) {
			second_generation_best_fitness = best_fitness;
		}

		/*if (n_generations_without_improvement >= 10) {
			population.write_result_to_file("..\\..\\solution_before_improvement.txt");
		}*/

		// give update, and iterate
		std::cout << "Nr. generation: " << generation << ", best fitness: " << best_fitness << " : " << best_fitness/static_cast<double>(fitness_solution) << "%" << std::endl;

		int i = 0;
		if (i == 0 && best_fitness < within30percent) {
			std::cout << "30% within best solution reached" << std::endl;
			population.write_result_to_file("..\\..\\30percent_solution.txt");
			i++;
		}
		else if (i == 1 && best_fitness < within20percent) {
			std::cout << "20% within best solution reached" << std::endl;
			population.write_result_to_file("..\\..\\20percent_solution.txt");
			i++;
		}
		else if (i == 2 && best_fitness < within10percent) {
			std::cout << "10% within best solution reached" << std::endl;
			population.write_result_to_file("..\\..\\10percent_solution.txt");
			i++;
		}
		else if (best_fitness < within05percent) {
			std::cout << "5% within best solution reached" << std::endl;
			population.write_result_to_file("..\\..\\05percent_solution.txt");
			break;
		}


		generation++;
	}
	std::cout << std::endl << std::endl << "Second generation best fitness: " << second_generation_best_fitness << ", best fitness at termination: " << best_fitness << std::endl;
	//population.print_population();

}

double exponential_decay(int index, int time_constant) {
	return pow(2.718, -time_constant / (index + 0.001));
}