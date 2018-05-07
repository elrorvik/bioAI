
#include <string>
#include <iostream>
#include"file.h"
#include "operation_manager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Operation_manager::Operation_manager(std::string filename) {

	std::fstream f(filename);
	if (!f.is_open()) {
		std::cout << "Could not find file path" << std::endl;
		return;
	}
	std::string line;
	std::stringstream ss;
	std::getline(f, line);
	ss << line;
	ss >> n_jobs;
	ss >> n_machines;
	ss.clear();

	int machine_id;
	double duration;
	operation_seq = operation_seq_t(n_jobs, std::vector<operation_t>());
	for (int job_id = 0; job_id < n_jobs; ++job_id) {
		std::getline(f, line);
		ss << line;
		int operation_id = 0;
		while (ss >> machine_id) {
			ss >> duration;
			operation_seq[job_id].push_back(operation_t(machine_id, operation_id, duration));
			operation_id++;
		}
		ss.clear();
	}
	current_job_index = std::vector<int>(n_jobs, 0);
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
		double finish_time = operation_seq[job_index][n_machines - 1].start_time + operation_seq[job_index][n_machines - 1].duration;
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
	operation_t a(1.0, 0, 1);
	operation_seq[0].push_back(a);
	operation_t b(1.0, 1, 2);
	operation_seq[0].push_back(b);
	operation_t c(1.0, 1, 3);
	operation_seq[0].push_back(c);

	operation_t e(1.0, 1, 1);
	operation_seq[1].push_back(e);
	operation_t f(1.0, 2, 2);
	operation_seq[1].push_back(f);
	operation_t g(1.0, 2, 3);
	operation_seq[1].push_back(g);

	operation_t c_1(1.0, 2, 1);
	operation_seq[2].push_back(c_1);
	operation_t c_2(1.0, 2, 2);
	operation_seq[2].push_back(c_2);
	operation_t c_3(1.0, 1, 3);
	operation_seq[2].push_back(c_3);

	current_job_index = std::vector<int>(n_jobs, 0);
	print_operation_sequence();
}

void test_JSSP_sol(const std::vector<int> &sol, int n_jobs, int n_machines) {
	std::vector<int> job_count(n_jobs, 0);
	for (int i = 0; i < sol.size(); i++) {
		for (int job = 0; job < n_jobs; job++) {
			if (sol[i] == job) job_count[job]++;
		}
	}

	for (int i = 0; i < n_jobs; i++) {
		if (job_count[i] != n_machines) {
			std::cout << "Error! JSSP solution does not contain correct amount of job with ID " << i << std::endl;
			std::cin.get();
		}
	}
}