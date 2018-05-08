#pragma once
#ifndef OPERATION_MANAGER_H
#define OPERATION_MANAGER_H
#include <vector>
#include <string>
#include <iostream>

struct interval_roulett {
	double start;
	double end;
	int job_index;
	bool inInterval(double val) {
		return (val >= start && val <= end);
	}

};

struct start_duration_pair {
	double start;
	double duration;
	start_duration_pair(double start, double duration) : start(start), duration(duration) {};
	bool operator<(const start_duration_pair& rhs) {
		return this->start < rhs.start;
	}
};

struct operation_t {
	double start_time;
	const double duration;
	const int machine_id;
	const int job_id;
	const int operation_id;
	operation_t(int machine_id,int task_id, int job_id, double duration) : start_time(-1.0), duration(duration), machine_id(machine_id), operation_id(task_id), job_id(job_id) {};
	operation_t(int machine_id, int task_id, int job_id,  double duration, double start_time) : start_time(start_time), duration(duration), machine_id(machine_id), operation_id(task_id), job_id(job_id) {};
	operation_t(const operation_t& rhs) :operation_t(rhs.machine_id, rhs.operation_id, rhs.job_id, rhs.duration, rhs.start_time) {}
};

typedef std::vector<std::vector<operation_t>> operation_seq_t;

class Operation_manager
{
private:
	int n_machines;
	int n_jobs;
	operation_seq_t operation_seq;
	std::vector<int> current_job_index;
public:
	Operation_manager(std::string file_name);

	int get_current_job_index(int job) { return current_job_index[job]; }// should be named get current task id
	int get_n_machines() { return n_machines; };
	int get_n_jobs() { return n_jobs; };

	int get_jobs_current_machine_id(int job_index) {
		return operation_seq[job_index][current_job_index[job_index]].machine_id;
	}
	double get_jobs_current_start_time(int job_index) { return operation_seq[job_index][current_job_index[job_index]].start_time; }
	double get_jobs_current_process_time(int job_index) { return operation_seq[job_index][current_job_index[job_index]].duration; }

	int get_op_size(int i) { return operation_seq[i].size(); }
	void increment(int job) { ++current_job_index[job]; }
	void set_job_start_time(int job_index, double start_time) { operation_seq[job_index][current_job_index[job_index]].start_time = start_time; };


	operation_t get_jobs_current_operation(int job_index) { return operation_seq[job_index][current_job_index[job_index]]; }
	operation_t get_job_at_machine_time(int machine_ID, double time);
	void resolve_task_overlap_at_machine(int machine_ID, std::vector<start_duration_pair>& machine_vacancies);
	std::vector<int> get_genotype();
	double get_jobs_prev_operation_start_time(int job_index) { return operation_seq[job_index][current_job_index[job_index] - 1].start_time; }
	double get_jobs_prev_operation_process_time(int job_index) { return operation_seq[job_index][current_job_index[job_index] - 1].duration; }
	bool job_not_complete(int job_index) { return !(current_job_index[job_index] == operation_seq[job_index].size()); }
	bool job_complete(int job_index) { return (current_job_index[job_index] == operation_seq[job_index].size()); }
	void reset_all_jobs() {
		for (int i = 0; i < n_jobs; i++) {
			current_job_index[i] = 0;
			operation_seq[i][0].start_time = 0.0;
		}
	}
	bool all_job_finished() {
		for (int i = 0; i < n_jobs; i++) {
			if (current_job_index[i] < operation_seq[i].size()) {
				return false;
			}
		}
		return true;
	}


	void print_operation_sequence();
	void reset_increment() { for (int i = 0; i < n_jobs; i++) current_job_index[i] = 0; }
	bool job_full(int job) { return (current_job_index[job] == operation_seq[job].size()); }

	double get_operation_finish_time();
	void test();
};

void test_JSSP_sol(const std::vector<int> &sol, int n_jobs, int n_machines);

#endif