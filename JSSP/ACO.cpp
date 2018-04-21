#include "operation_manager.h"
#include <vector>
#include <iostream>
#include "ACO.h"


void ACO(Operation_manager& om) {


	int n = om.get_n_jobs(); // number of jobs
	int m = om.get_n_machines(); // number of machines
	int n_ants = n / 2; // number of ants

	vec_float_3d phi; // pheromone
	vec_float_3d delta_phi; //pheromone
	initialize_pheromone(om, phi, delta_phi);
	

}

void initialize_pheromone(Operation_manager& om, vec_float_3d& phi, vec_float_3d& delta_phi) {
	int n = om.get_n_jobs(); // number of jobs
	vec_float_1d pher_j(n, 0);
	vec_float_1d delta_j(n, 0);

	for (int job_i = 0; job_i < n; job_i++) {
		vec_float_2d pher_i;
		vec_float_2d delta_i;
		for (int task_i = 0; task_i <= om.get_op_size(job_i); task_i++) {
			for (int i = 0; i < n; i++) {
				pher_j[i] = (((double)rand() / (RAND_MAX / PHERO_INIT)));
			}
			pher_i.push_back(pher_j);
			delta_i.push_back(delta_j);
		}
		phi.push_back(pher_i);
		delta_phi.push_back(delta_i);
	}

}