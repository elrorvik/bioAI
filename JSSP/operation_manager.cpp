
#include <string>
#include <iostream>
#include"file.h"
#include "operation_manager.h"

Operation_manager::Operation_manager(std::string filename) {

	//read_file(filename, this->n_jobs, this->n_machines, this->operation_seq);
	//current_job_index = std::vector<int>(n_jobs, 0);
	//print_operation_sequence();
	test();
}

void Operation_manager::print_operation_sequence() {
	std::cout << "N jobs: \t" << n_jobs << std::endl;
	std::cout << "N machines: \t" << n_machines << std::endl;
	
	for (int job_id = 0; job_id  < n_jobs; ++job_id) {
		std::cout << "***** Job ID:" << job_id << " : *****" << std::endl;
		std::cout << "Current operation id: " << current_job_index[job_id] << std::endl;
		for (auto it = operation_seq[job_id].begin(); it != operation_seq[job_id].end(); ++it) {
			std::cout << "Operation id: " << it->operation_id <<  " ,Machine id: " <<it->machine_id << " ,Duration: " << it->duration << " ,Start time: " << it->start_time <<  std::endl;
		}
	}
}

double Operation_manager::get_operation_finish_time() {
	double max_finish_time = -DBL_MAX;
	for (int job_index = 0; job_index < n_jobs; job_index++) {
		double finish_time = operation_seq[job_index][n_jobs - 1].start_time + operation_seq[job_index][n_jobs - 1].duration;
		if (finish_time > max_finish_time) {
			max_finish_time = finish_time;
		}
	}
	return max_finish_time;
}

void Operation_manager::test() {
	n_machines = 3;
	n_jobs = 3;
	// double duration, int machine_id, int task_id
	operation_seq = operation_seq_t(n_jobs, std::vector<operation_t>());
	operation_t a(1.0, 1, 1);
	operation_seq[0].push_back(a);
	operation_t b(1.0, 2, 2);
	operation_seq[0].push_back(b);
	operation_t c(1.0, 2, 3);
	operation_seq[0].push_back(c);

	operation_t e(1.0, 2, 1);
	operation_seq[1].push_back(e);
	operation_t f(1.0, 3, 2);
	operation_seq[1].push_back(f);
	operation_t g(1.0, 3, 3);
	operation_seq[1].push_back(g);

	operation_t c_1(1.0, 3, 1);
	operation_seq[2].push_back(c_1);
	operation_t c_2(1.0, 3, 2);
	operation_seq[2].push_back(c_2);
	operation_t c_3(1.0, 2, 3);
	operation_seq[2].push_back(c_3);

	current_job_index = std::vector<int>(n_jobs, 0);
	print_operation_sequence();
}