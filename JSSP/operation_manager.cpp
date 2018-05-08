
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
			operation_seq[job_id].push_back(operation_t(machine_id, operation_id, job_id, duration));
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
	operation_t a(1.0, 0, 1, 0);
	operation_seq[0].push_back(a);
	operation_t b(1.0, 1, 2, 0);
	operation_seq[0].push_back(b);
	operation_t c(1.0, 1, 3, 0);
	operation_seq[0].push_back(c);

	operation_t e(1.0, 1, 1, 1);
	operation_seq[1].push_back(e);
	operation_t f(1.0, 2, 2, 1);
	operation_seq[1].push_back(f);
	operation_t g(1.0, 2, 3, 1);
	operation_seq[1].push_back(g);

	operation_t c_1(1.0, 2, 1, 2);
	operation_seq[2].push_back(c_1);
	operation_t c_2(1.0, 2, 2, 2);
	operation_seq[2].push_back(c_2);
	operation_t c_3(1.0, 1, 3, 2);
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

 operation_t Operation_manager::get_job_at_machine_time(int machine_ID, double time) {
	for (int job = 0; job < n_jobs; job++) {
		for (int task = 0; task < n_machines; task++) {
			if (operation_seq[job][task].start_time <= time && operation_seq[job][task].start_time + operation_seq[job][task].duration >= time) {
				return operation_seq[job][task];
			}
		}
	}
	return operation_t(-1, -1, -1, -1);
}

 void Operation_manager::resolve_task_overlap_at_machine(int machine_ID, std::vector<start_duration_pair>& vacancies, std::vector<double>& jobs_finish_time) {
	 double min_duration = DBL_MAX;
	 double total_time = 0;
	 for (int task = 0; task < n_machines; task++) {
		 for (int job = 0; job < n_jobs; job++) {
			 double task_duration = operation_seq[job][task].duration;
			 if (task_duration < min_duration) min_duration = task_duration;
			 total_time += task_duration;
		 }
	 }

	 double current_time = 0;
	 double prev_start_time = -1.0;
	 double prev_duration;
	 while (current_time < total_time) {
		 operation_t task = get_job_at_machine_time(machine_ID, current_time);
		 if (task.job_id == -1) {
			 current_time += min_duration;
			 continue;
		 }
		 if (prev_start_time == -1.0) {
			 prev_start_time = task.start_time;
			 prev_duration = task.duration;
			 current_time += min_duration;
			 continue;
		 }

		 if (prev_start_time + prev_duration > task.start_time) {
			 operation_seq[task.job_id][task.operation_id].start_time = prev_start_time + prev_duration;
		 }

		 prev_start_time = task.start_time;
		 prev_duration = task.duration;
		 current_time += min_duration;
	 }
	 double finish_time = prev_start_time + prev_duration;

	 // Create new vacancies, possibly through another function
	 prev_start_time = -1.0;
	 vacancies.clear();
	 double current_machine_finish_time = 0.0;
	 for (int task = 0; task < n_machines; task++) {
		 for (int job = 0; job < n_jobs; job++) {
			 if (machine_ID == operation_seq[job][task].machine_id) {
				 if (prev_start_time == -1.0) {
					 prev_start_time = operation_seq[job][task].start_time;
					 prev_duration = operation_seq[job][task].duration;
					 continue;
				 }

				 if (prev_start_time + prev_duration + prev_duration/100000 < operation_seq[job][task].start_time) {
					 vacancies.emplace_back(prev_start_time + prev_duration, operation_seq[job][task].start_time - prev_start_time + prev_duration);
				 }

				 if (operation_seq[job][task].start_time + operation_seq[job][task].duration > current_machine_finish_time) {
					 current_machine_finish_time = operation_seq[job][task].start_time + operation_seq[job][task].duration;
				 }

				 prev_start_time = operation_seq[job][task].start_time;
				 prev_duration = operation_seq[job][task].duration;
			 }
		 }
	 }
	 vacancies.emplace_back(current_machine_finish_time, DBL_MAX);

	 // Keep track of the last current finish time of each job
	 for (int task = 0; task < n_machines; task++) {
		 for (int job = 0; job < n_jobs; job++) {
			 if (operation_seq[job][task].start_time > jobs_finish_time[job]) {
				 jobs_finish_time[job] = operation_seq[job][task].start_time;
			 }
		 }
	 }
 }

std::vector<int> Operation_manager::get_genotype() {
	std::vector<int> genotype(n_jobs*n_machines, 0);
	double min_duration = DBL_MAX;
	for (int task = 0; task < n_machines; task++) {
		for (int job = 0; job < n_jobs; job++) {
			double task_duration = operation_seq[job][task].duration;
			if (task_duration < min_duration) min_duration = task_duration;
		}
	}

	int added_jobs = 0;
	double current_time = 0.0;
	while (added_jobs < n_machines*n_jobs) {
		
		for (int job = 0; job < n_jobs; job++) {
			if (job_complete(job)) continue;
			double task_start = get_jobs_current_start_time(job);
			if (task_start >= current_time && task_start <= current_time + min_duration) {
				genotype[added_jobs++] = get_jobs_current_operation(job).job_id;
				increment(job);
			}
		}
		current_time += min_duration;

	}
	reset_increment();
	return genotype;
}