#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "ACO.h"
#include "scheduler.h"
#include <algorithm>
using namespace std;

struct Ant {
	std::vector<int> tasks;
	float fitness;

	bool operator <(const Ant& rhs) {
		return fitness < rhs.fitness;
	}
};



void init_pheromone(Operation_manager& jobs, vector<vector<vector<float>>>& phi, vector<vector<vector<float>>>& delta_phi, int& total_operations);
void calc_start_time(Operation_manager& tabu, operation_seq_t& schedule);
int pin_on_roulette(vector<interval_roulett>& roulette);
void make_roulette_board(Operation_manager& tabu, vector<interval_roulett>& roulette, vector<float>& P);
void calc_transition_probability(Operation_manager& tabu, vector<Ant>& colony, vector<vector<vector<float>>>& phi, int ant_k, vector<float>& P, double ALPHA, double BETHA);
double eta_LPT(double SC, double PT);
double eta_SPT(double SC, double PT);
void calc_start_time(Operation_manager& tabu, std::vector<std::vector<start_duration_pair>>& machine_vacancies_tab);
void update_vacencies(Operation_manager& tabu, std::vector<std::vector<start_duration_pair>>& machine_vacancies_tab, int job_id);


void ant_coloy_optimization(Operation_manager& om, float target) {
	int n_jobs = om.get_n_jobs();
	int n_machines = om.get_n_machines();
	const int K_ANTS = 500; //n_jobs / 2;

	int full_length_geno = 0;
	vector<vector<vector<float>>> pher; //pheromone
	vector<vector<vector<float>>> pher_delta; //pheromone added to old pheromone
	init_pheromone(om,  pher, pher_delta, full_length_geno);

	vector<Ant> colony(K_ANTS);
	Ant bestSolution;
	bestSolution.fitness = INFINITY;
	Operation_manager om_init = om;

	int iterations = 0;
	int percent_20 = 0;
	int percent_30 = 0;
	int percent_10 = 0;

	while (1) {
		for (int ant_k = 0; ant_k < K_ANTS; ++ant_k) {

			std::vector<std::vector<start_duration_pair>> machine_vacancies_tab;
			for (int m_index = 0; m_index < om.get_n_machines(); m_index++) {
				machine_vacancies_tab.emplace_back();
				machine_vacancies_tab[m_index].emplace_back(0, DBL_MAX);
			}

			double ALPHA = (rand() % 99) / 100 + 0.01;
			double BETHA = 1.0 - ALPHA;
			Operation_manager tabu = om_init; 
			colony[ant_k].tasks.clear();

			//random assignment of first task
			colony[ant_k].tasks.push_back(rand() % n_jobs);
			tabu.increment(colony[ant_k].tasks[0]);

			// schedule of which operations are performed at machines
			//operation_seq_t schedule(tabu.get_n_machines(), vector<operation_t>());
			while (colony[ant_k].tasks.size() < full_length_geno) {
				//calculations for stepcounter
				//calc_start_time( tabu, schedule);
				calc_start_time(tabu,machine_vacancies_tab);
			
				vector<float> P(n_jobs, 0.0);
				calc_transition_probability(tabu, colony, pher, ant_k, P, ALPHA,BETHA);

				vector<interval_roulett> roulette;
				make_roulette_board(tabu, roulette, P);
				int roulette_id = pin_on_roulette(roulette);

				update_vacencies(tabu, machine_vacancies_tab, roulette_id);
				//schedule.at(tabu.get_jobs_current_machine_id(roulette_id)).push_back(tabu.get_jobs_current_operation(roulette_id));
				//tabu.increment(roulette_id);
				colony[ant_k].tasks.push_back(roulette_id);
			}
			colony[ant_k].fitness = calc_makespan(tabu, colony[ant_k].tasks);

			int scale = 1;
			if (colony[ant_k] < bestSolution) {
				bestSolution = colony[ant_k];
				scale = 1;
			}

			// calculate pheromone
			vector<char> current_job_index(n_jobs, 0);
			for (int i = 1; i < colony[ant_k].tasks.size(); ++i) {
				if (current_job_index[colony[ant_k].tasks[i - 1]]> n_machines) continue;
				int prev_job = colony[ant_k].tasks[i - 1];
				int prev_task = current_job_index[colony[ant_k].tasks[i - 1]];
				int current_job = colony[ant_k].tasks[i];
				pher_delta[prev_job][prev_task][current_job] += Q *scale/ colony[ant_k].fitness;
				current_job_index[colony[ant_k].tasks[i]]++;
			}
		}

		//Update pheromone globally
		for (int i = 0; i < n_jobs; ++i) {
			for (int j = 0; j <= n_machines; ++j) {
				for (int edge_index = 0; edge_index < n_jobs; ++edge_index) {
					float new_phero = pher_delta[i][j][edge_index] + RHO * pher[i][j][edge_index];
					if (pher[i][j][edge_index] < PHERO_LIMIT_MIN) new_phero = PHERO_LIMIT_MIN;
					if (pher[i][j][edge_index] > PHERO_LIMIT_MAX) new_phero = PHERO_LIMIT_MAX;
					pher[i][j][edge_index] = new_phero;
					pher_delta[i][j][edge_index] = 0;
				}
			}
		}
		
		if (target*1.3 > bestSolution.fitness && percent_30 == 0) {
			cout << "within 30% " << bestSolution.fitness << std::endl;
			percent_30 = 1;
		}
		else if (target*1.2 > bestSolution.fitness && percent_20 == 0) {
			cout << "within 20% " << bestSolution.fitness << std::endl;
			percent_20 = 1;
		}
		else if (percent_20 == 1 && iterations % 100 == 0) {
			std::cout << bestSolution.fitness << " ";
		}
		else if (target*1.1 > bestSolution.fitness) {
			cout << "on target within 10% " << bestSolution.fitness << std::endl;
			break;
		}
		iterations++;
	}
}

void init_pheromone(Operation_manager& om, vector<vector<vector<float>>>& pher, vector<vector<vector<float>>>& pher_delta, int& total_operations) {

	const int n_jobs = om.get_n_jobs();
	vector<float> delta_task(n_jobs, 0);
	vector<float> phi_task(n_jobs, 0);
	for (int job = 0; job < n_jobs; ++job) {
		vector<vector<float>> phi_job;
		vector<vector<float>> delta_job;
		total_operations += om.get_op_size(job);
		for (int task = 0; task <= om.get_op_size(job); ++task) {
				
			for (int edge = 0; edge < n_jobs; ++edge) {
				phi_task[edge] = (((double)rand() / (RAND_MAX / PHERO_INIT)));
			}
			phi_job.push_back(phi_task);
			delta_job.push_back(delta_task);
		}
		pher.push_back(phi_job);
		pher_delta.push_back(delta_job);
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


void calc_start_time(Operation_manager& tabu,  std::vector<std::vector<start_duration_pair>>& machine_vacancies_tab) {
	for (int job_id = 0; job_id < tabu.get_n_jobs(); job_id++) {
		if (tabu.job_complete(job_id)) continue; // if assignment complete, continue

		float job_finish_time = 0;
		// get prev jobs end time, meaning earliest starting point
		if (tabu.get_current_job_index(job_id) > 0) {
			job_finish_time = tabu.get_jobs_prev_operation_start_time(job_id) + tabu.get_jobs_prev_operation_process_time(job_id);
		}

		double task_duration = tabu.get_jobs_current_process_time(job_id);
		int task_machine_ID = tabu.get_jobs_current_machine_id(job_id);
		std::vector<start_duration_pair>* machine_vacancies = &machine_vacancies_tab[task_machine_ID];

		for (int vac_index = 0; vac_index < machine_vacancies->size(); vac_index++) {
			if ((*machine_vacancies)[vac_index].start + (*machine_vacancies)[vac_index].duration < job_finish_time) continue;

			double first_available_start_time = 0;
			double available_processing_time;
			if ((*machine_vacancies)[vac_index].start > job_finish_time) {
				available_processing_time = (*machine_vacancies)[vac_index].duration;
				first_available_start_time = (*machine_vacancies)[vac_index].start;
			}
			else {
				available_processing_time = (*machine_vacancies)[vac_index].duration - (job_finish_time - (*machine_vacancies)[vac_index].start);
				first_available_start_time = job_finish_time;
			}

			if (task_duration <= available_processing_time) {
				job_finish_time = first_available_start_time + task_duration;
				tabu.set_job_start_time(job_id, job_finish_time);
				break;

				
			}
		}
	}
}

void update_vacencies(Operation_manager& tabu,std::vector<std::vector<start_duration_pair>>& machine_vacancies_tab, int job_id) {

	float job_finish_time = 0;
	if (tabu.get_current_job_index(job_id) > 0) {
		job_finish_time = tabu.get_jobs_prev_operation_start_time(job_id) + tabu.get_jobs_prev_operation_process_time(job_id);
	}

	double task_duration = tabu.get_jobs_current_process_time(job_id);
	int task_machine_ID = tabu.get_jobs_current_machine_id(job_id);
	std::vector<start_duration_pair>* machine_vacancies = &machine_vacancies_tab[task_machine_ID];

	for (int vac_index = 0; vac_index < machine_vacancies->size(); vac_index++) {
		if ((*machine_vacancies)[vac_index].start + (*machine_vacancies)[vac_index].duration < job_finish_time) continue;

		double first_available_start_time = 0;
		double available_processing_time;
		if ((*machine_vacancies)[vac_index].start > job_finish_time) {
			available_processing_time = (*machine_vacancies)[vac_index].duration;
			first_available_start_time = (*machine_vacancies)[vac_index].start;
		}
		else {
			available_processing_time = (*machine_vacancies)[vac_index].duration - (job_finish_time - (*machine_vacancies)[vac_index].start);
			first_available_start_time = job_finish_time;
		}

		if (task_duration <= available_processing_time) {

			job_finish_time = first_available_start_time + task_duration;

			tabu.set_job_start_time(job_id, first_available_start_time);
			tabu.increment(job_id);

			// Update vacancies
			if (first_available_start_time != (*machine_vacancies)[vac_index].start) { // Then create a new vacancy
				double new_vac_duration = first_available_start_time - (*machine_vacancies)[vac_index].start;
				(*machine_vacancies).emplace_back(first_available_start_time + task_duration, (*machine_vacancies)[vac_index].duration - new_vac_duration - task_duration);
				(*machine_vacancies)[vac_index].duration = new_vac_duration;
				//(*machine_vacancies)[vac_index].start unchanged
				std::sort((*machine_vacancies).begin(), (*machine_vacancies).end());
			}
			else {
				(*machine_vacancies)[vac_index].start += task_duration;
				if ((*machine_vacancies)[vac_index].duration != DBL_MAX) (*machine_vacancies)[vac_index].duration -= task_duration;
			}

			if ((*machine_vacancies)[vac_index].duration <= task_duration / 10000000) {
				(*machine_vacancies).erase((*machine_vacancies).begin() + vac_index);
			}
			break; 
		}
	}
}



void make_roulette_board(Operation_manager& tabu, vector<interval_roulett>& roulette,  vector<float>& P) {
	float value = 0; // between 0 and 1
	interval_roulett temp;
	for (int job = 0; job < tabu.get_n_jobs(); ++job) {
		if (tabu.job_complete(job)) continue;
		temp.start = value;
		value += P[job];
		temp.end = value;
		temp.job_index = job;
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

void calc_transition_probability(Operation_manager& tabu, vector<Ant>& colony, vector<vector<vector<float>>>& pher, int ant_k, vector<float>& P, double ALPHA, double BETHA) {

	int n_jobs = tabu.get_n_jobs();
	vector<float> tau_ij(n_jobs, 0.0);
	float tau_sum = 0.0;
	for (int next = 0; next < n_jobs; ++next) {
		if (tabu.job_complete(next)) continue;
		int current_job = colony[ant_k].tasks.back();
		tau_ij[next] = pher[current_job][tabu.get_current_job_index(current_job)][next];
		double step = 0;
		if (tabu.get_jobs_current_start_time(next) == 0) {
			step = 1;
		}
		else {
			step = tabu.get_jobs_current_start_time(next);
		}
		double PT = tabu.get_jobs_current_process_time(next);
		tau_sum += pow(tau_ij[next], ALPHA)*pow(eta_SPT(step, PT), BETHA);
	}
	
	for (int next = 0; next< n_jobs; ++next) {
		if (tabu.job_complete(next)) continue;
		double step = 0;
		if (tabu.get_jobs_current_start_time(next) == 0) {
			step = 1;
		}
		else {
			step = tabu.get_jobs_current_start_time(next);
		}
		double PT = tabu.get_jobs_current_process_time(next);
		P[next] = (pow(tau_ij[next], ALPHA)*pow(eta_SPT(step,PT), BETHA) / tau_sum);
	}
}


double eta_SPT(double SC, double PT) {
	int eta_Q = 1;
	return eta_Q / (SC * PT);
}

double eta_LPT(double SC, double PT) {
	int eta_Q = 5;
	return eta_Q * PT / (SC + PT);
}