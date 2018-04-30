#include <iostream>
#include "ACO.h"
const int PHERO_INIT = 10;
const int A_MAX = 100; // max number of iteration before updating pheromone



void ACO(Operation_manager& om) {


	int n = om.get_n_jobs(); // number of jobs
	int m = om.get_n_machines(); // number of machines
	int n_ants = n / 2; // number of ants
	int b_max = n * m; // total number of steps to generate a resulting  disjunctive graph

	vec_float_3d phi; // pheromone
	vec_float_3d delta_phi; //pheromone
	int total_operations;
	initialize_pheromone(om, phi, delta_phi, total_operations);
	//print_vector(phi);
	//print_vector(delta_phi);
	

	std::vector<std::vector<int>> colony(n_ants);
	std::vector<double> colony_finish_time(n_ants);
	std::vector<int> best_geno;
	float best_finish_time = INFINITY;

	Operation_manager init_om = om;


	int iteration = 100;
	while (iteration) {
		for (int ant_i = 0; ant_i < n_ants; ant_i++) {

			Operation_manager tabu = init_om; // all decision for the spesific ant ant_i
			colony[ant_i].clear();

			// Random assignment of first operation
			int rand_first_node = rand() % n;
			colony[ant_i].push_back(rand_first_node);
			tabu.increment(rand_first_node); // increment the first operation in the tabu list
			iteration = 0;

			// while tabu not filled
			operation_seq_t schedule(m, std::vector<operation_t>());
			while (colony[ant_i].size() < total_operations) {

				// need calculate start time

				std::vector<double> tau_ij(n, 0.0);
				
				float sum_tau = 0.0;


				// calculating pheromone
				for (int i = 0; i < tabu.get_n_jobs(); i++) {
					if (tabu.job_full(i)) continue; // look at why
					int op_i = colony[ant_i].back();
					std::cout << op_i << " vs." << phi.size() << std::endl;
					std::cout << tabu.get_current_job_index(op_i) << " vs." << phi[op_i].size() << std::endl;
					std::cout << i << " vs. " << phi[op_i][tabu.get_current_job_index(op_i)].size() << std::endl;
					tau_ij[i] += phi[op_i][tabu.get_current_job_index(op_i)][i];
					
					sum_tau += pow(tau_ij[i], ALPHA)*pow(eta(tabu,i), BETHA);
				}
				
				// Making rulett
				double tot_value = 0;
				std::vector<interval_roulett> roulette;
				for (int i = 0; i < tabu.get_n_jobs(); i++) {
					if (tabu.job_full(i)) continue; // look at why
					interval_roulett pair;
					pair.start = tot_value;
					tot_value += pow(tau_ij[i], ALPHA)*pow(eta(tabu, i), BETHA)/sum_tau;
					pair.end = tot_value;
					pair.job_index = i;
					roulette.push_back(pair);

				}
				roulette.back().end = 1.0; // because of desimal error

				// roulette
				int rand_job_id;
				double rand_num = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
				for (int i = 0; i < roulette.size(); i++) {
					if (roulette[i].inInterval(rand_num)) {
						rand_job_id = roulette[i].job_index;
						break;
					}
				}
				schedule.at(tabu.get_jobs_current_machine_id(rand_job_id));
				tabu.increment(rand_job_id);
				colony[ant_i].push_back(rand_job_id);


			}

			// TODO: calculate finish time
			colony_finish_time[ant_i] = 0.0;
			

			if (colony_finish_time[ant_i] < best_finish_time) {
				best_geno = colony[ant_i];
			}

			// update local pheromone trail, eq. 3
			update_delta_phi(colony, colony_finish_time, delta_phi, ant_i, m, n);
			
		}
		// eq 2, global update from delta phi to phi
		for (int job = 0; job < n; job++) {
			for (int machine = 0; machine < m; machine++) {
				for (int edge_i = 0; edge_i < n; edge_i++) {
					phi[job][machine][edge_i] = delta_phi[job][machine][edge_i] + RHO * phi[job][machine][edge_i];
					if (phi[job][machine][edge_i] < PHERO_MIN) phi[job][machine][edge_i] = PHERO_MIN;
					if (phi[job][machine][edge_i] < PHERO_MIN) phi[job][machine][edge_i] = PHERO_MAX;

					delta_phi[job][machine][edge_i] = 0; // reset delta_phi
				}
			}
		}

		// Remember the most optimal solutsion
		for (int ant = 0; ant < n_ants; ant++) {
			// local search on colony[k], some number , jobs
			if (colony_finish_time[ant] < best_finish_time) {
				best_geno = colony[ant];
				update_delta_phi(colony, colony_finish_time, delta_phi, ant, m, n);
				
			}
		}

		// add functionality to add iterations;
		iteration--;

	}

}

void update_delta_phi(std::vector<std::vector<int>> &colony, std::vector<double> &colony_finish_time, vec_float_3d& delta_phi, int ant_i, int m, int n) {
	std::vector<int> current_job_index(n, 0);
	for (int i = 1; i < colony[ant_i].size(); i++) {
		if (current_job_index[colony[ant_i][i - 1]] > m); continue;
		int prev_op = colony[ant_i][i - 1];
		int next_op = colony[ant_i][i];
		delta_phi[prev_op][current_job_index[prev_op]][next_op] += Q / colony_finish_time[ant_i];
		current_job_index[next_op]++;
	}
}

double eta(Operation_manager& tabu, int job) {
	return tabu.get_jobs_current_process_time(job) + tabu.get_jobs_current_process_time(job);
}

void initialize_pheromone(Operation_manager& om, vec_float_3d& phi, vec_float_3d& delta_phi, int &total_iteartions) {
	int n = om.get_n_jobs(); // number of jobs

	vec_float_1d delta_j(n, 0);
	vec_float_1d pher_j(n, 0.0);
	
	total_iteartions = 0;

	for (int job_i = 0; job_i < n; job_i++) {
		total_iteartions += n * om.get_op_size(job_i);
		vec_float_2d pher_i;
		vec_float_2d delta_i;

		for (int task_i = 0; task_i <= om.get_op_size(job_i); task_i++) {			
			for (int j = 0; j < n; j++) {
				pher_j[j] = ((double)rand() / (RAND_MAX / PHERO_INIT));
			}
			delta_i.push_back(delta_j);
			pher_i.push_back(pher_j);
		}
		phi.push_back(pher_i);
		delta_phi.push_back(delta_i);
	}
}

void print_vector(vec_float_3d& vector) {
	for (int i = 0; i < vector.size(); i++) {
		std::cout << "Job: " << i << " " << std::endl;
		for (int j = 0; j < vector[i].size(); j++) {
			for (int k = 0; k < vector[i][j].size(); k++) {
				std::cout << vector[i][j][k] << " ";
			}
			std::cout << std::endl;
		}
	}
}