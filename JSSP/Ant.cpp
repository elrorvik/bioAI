#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "Ant.h"
#include "scheduler.h"
#include <algorithm>
using namespace std;

struct Ant {
	std::vector<int> gene;
	float finish_time;

	bool operator <(const Ant& rhs) {
		return finish_time < rhs.finish_time;
	}
};



void init_pheromone(Operation_manager& jobs, vector<vector<vector<float>>>& phi, vector<vector<vector<float>>>& delta_phi, int& total_operations);
void calc_start_time(Operation_manager& tabu, operation_seq_t& schedule);
void make_roulette_board(Operation_manager& tabu, vector<interval_roulett>& roulette, float tau_sum, vector<float>& tau_ij);
int pin_on_roulette(vector<interval_roulett>& roulette);

void ants(Operation_manager& om, float target) {

	int n_jobs = om.get_n_jobs();
	int m = om.get_n_machines();
	const int K_ANTS = 500; //n_jobs / 2;

	int total_operations = 0;
	vector<vector<vector<float>>> phi; //pheromone
	vector<vector<vector<float>>> delta_phi; //pheromone

	init_pheromone(om,  phi,  delta_phi,  total_operations);

	vector<Ant> colony(K_ANTS);
	Ant bestSolution;
	bestSolution.finish_time = INFINITY;
	Operation_manager om_init = om;

	int itterations = 0;
	int percent_10 = 0;
	int percent_20 = 0;
	int percent_30 = 0;

	while (1) {
		for (int ant_k = 0; ant_k < K_ANTS; ++ant_k) {
			Operation_manager tabu = om_init; 
			colony[ant_k].gene.clear();

			//random assignment of first task
			colony[ant_k].gene.push_back(rand() % n_jobs);
			tabu.increment(colony[ant_k].gene[0]);

			// schedule the remainign tasks
			operation_seq_t schedule(tabu.get_n_machines(), vector<operation_t>());

			while (colony[ant_k].gene.size() < total_operations) {
				//calculations for stepcounter
				calc_start_time( tabu, schedule);
			
				// pre values for pheromone
				vector<float> tau_ij(n_jobs, 0.0);
				float tau_sum = 0.0;
				for (int edge = 0; edge < n_jobs; ++edge) {
					if (tabu.job_complete(edge)) continue;
					int current_job = colony[ant_k].gene.back();
					tau_ij[edge] += phi[current_job][tabu.get_current_job_index(current_job)][edge];
					tau_sum += pow(tau_ij[edge], ALPHA)*pow(1 / (tabu.get_jobs_current_start_time(edge) + tabu.get_jobs_current_process_time(edge)), BETHA);
				}
				vector<interval_roulett> roulette;
				make_roulette_board(tabu, roulette, tau_sum, tau_ij);
				int roulette_id = pin_on_roulette(roulette);

				schedule.at(tabu.get_jobs_current_machine_id(roulette_id)).push_back(tabu.get_jobs_current_operation(roulette_id));
				tabu.increment(roulette_id);
				colony[ant_k].gene.push_back(roulette_id);
			}
			colony[ant_k].finish_time = calc_makespan(tabu, colony[ant_k].gene);

			
			if (colony[ant_k] < bestSolution) {
				bestSolution = colony[ant_k];
			}

			// calculate pheromone
			vector<char> current_job_index(n_jobs, 0);
			for (int i = 1; i < colony[ant_k].gene.size(); ++i) {

				if (current_job_index[colony[ant_k].gene[i - 1]]> m) continue;
				delta_phi[colony[ant_k].gene[i - 1]][current_job_index[colony[ant_k].gene[i - 1]]][colony[ant_k].gene[i]] += Q / colony[ant_k].finish_time; 

				current_job_index[colony[ant_k].gene[i]]++;
			}
		}

		//Update pheromone globally
		for (int i = 0; i < n_jobs; ++i) {
			for (int j = 0; j <= m; ++j) {
				for (int edge_index = 0; edge_index < n_jobs; ++edge_index) {
					phi[i][j][edge_index] = delta_phi[i][j][edge_index] + RHO * phi[i][j][edge_index]; 
					if (phi[i][j][edge_index] < PHERO_MIN) phi[i][j][edge_index] = PHERO_MIN;
					if (phi[i][j][edge_index] > PHERO_MAX) phi[i][j][edge_index] = PHERO_MAX;

					delta_phi[i][j][edge_index] = 0; 
				}
			}
		}
		
		if (target*1.3 > bestSolution.finish_time && percent_30 == 0) {
			cout << "within 30% " << bestSolution.finish_time << std::endl;
			percent_30 = 1;
		}
		else if (target*1.2 > bestSolution.finish_time && percent_20 == 0) {
			cout << "within 20% " << bestSolution.finish_time << std::endl;
			percent_20 = 1;
		}
		else if (percent_20 == 1 && itterations % 100 == 0) {
			std::cout << bestSolution.finish_time << " ";
		}else if (target*1.1 > bestSolution.finish_time && percent_10 == 0) {
			cout << "within 10% " << bestSolution.finish_time << std::endl;
			percent_10 = 1;
		}
		else if (percent_10 == 1 && itterations % 100 == 0) {
			std::cout << bestSolution.finish_time << " " ;
		}
		else if (target >= bestSolution.finish_time) {
			std::cout << "on target " << bestSolution.finish_time << " " << std::endl;
			break;
		}
		
		
		
		itterations++;
	}
}

void init_pheromone(Operation_manager& jobs, vector<vector<vector<float>>>& phi, vector<vector<vector<float>>>& delta_phi, int& total_operations) {
	const int n = jobs.get_n_jobs();
	for (char job = 0; job < n; ++job) {
		vector<vector<float>> pher_i;
		vector<vector<float>> delta_i;
			for (int task = 0; task <= jobs.get_op_size(job); ++task) {
			vector<float> pher_j;
			vector<float> delta_j;
			total_operations = n * jobs.get_op_size(job);
				for (int j = 0; j < n; ++j) {
					pher_j.push_back(((double)rand() / (RAND_MAX / PHERO_INIT)));
					delta_j.push_back(0);
				}
				pher_i.push_back(pher_j);
				delta_i.push_back(delta_j);
			}
		phi.push_back(pher_i);
		delta_phi.push_back(delta_i);
	}
}

void calc_start_time(Operation_manager& tabu, operation_seq_t& schedule) {
	for (int i = 0; i < tabu.get_n_jobs(); ++i) {
		
		if (tabu.job_complete(i)) continue; // if assignment complete, continue

		float job_end_time = 0;
		float machine_end_time = 0;
		// get job start time
		if (tabu.get_current_job_index(i) > 0) {
			job_end_time = tabu.get_jobs_prev_operation_start_time(i) + tabu.get_jobs_prev_operation_process_time(i);
		}
		//check if jobs at current machine
		int machine_size = schedule.at(tabu.get_jobs_current_machine_id(i)).size();

		if (machine_size) {
			machine_end_time = schedule.at(tabu.get_jobs_current_machine_id(i))[machine_size - 1].start_time +
				schedule.at(tabu.get_jobs_current_machine_id(i))[machine_size - 1].duration;
		}
		double new_start_time = max(job_end_time, machine_end_time); // change logic to come in between?
		tabu.set_job_start_time(i, new_start_time);
	}
}

void make_roulette_board(Operation_manager& tabu, vector<interval_roulett>& roulette, float tau_sum,vector<float>& tau_ij) {
	float totVal = 0;
	interval_roulett temp;
	for (int i = 0; i < tabu.get_n_jobs(); ++i) {
		if (tabu.job_complete(i)) continue;
		temp.start = totVal;
		totVal += (pow(tau_ij[i], ALPHA)*pow(1 / (tabu.get_jobs_current_start_time(i) + tabu.get_jobs_current_process_time(i)), BETHA) / tau_sum);
		temp.end = totVal;
		temp.job_index = i;
		roulette.push_back(temp);
	}
	roulette.back().end = 1;
}

int pin_on_roulette(vector<interval_roulett>& roulette) {
	int roulette_id = -1;
	float pin = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	for (int i = 0; i < roulette.size(); ++i) {
		if (roulette[i].inInterval(pin)) {
		roulette_id = roulette[i].job_index;
		break;
		}
	}
	return roulette_id;
}