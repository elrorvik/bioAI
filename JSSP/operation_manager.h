#pragma once
#ifndef OPERATION_MANAGER_H
#define OPERATION_MANAGER_H
#include <vector>
#include <string>

struct interval_roulett {
	double start;
	double end;
	int job_index;
	bool inInterval(double val) {
		return (val >= start && val <= end);
	}

};

struct operation_t {
	double start_time;
	const double duration;
	const int machine_id;
	const int operation_id;
	operation_t(double duration, int machine_id, int task_id) : start_time(-1.0), duration(duration), machine_id(machine_id), operation_id(task_id) {};
	operation_t(double start_time,double duration, int machine_id, int task_id) : start_time(start_time), duration(duration), machine_id(machine_id), operation_id(task_id) {};
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
	void print_operation_sequence();
	int get_n_machines() { return n_machines; };
	int get_n_jobs() { return n_jobs; };
	int get_op_size(int i) { return operation_seq[i].size(); }
	void increment(int job) { ++current_job_index[job];}
	void reset_increment() { for (int i = 0; i < n_jobs; i++) current_job_index[i] = 0; }
	bool job_full(int job) { return (current_job_index[job] == operation_seq[job].size()); }
	int get_current_job_index(int job) { return current_job_index[job]; }
	double get_jobs_current_start_time(int job_index) { return operation_seq[job_index][current_job_index[job_index]].start_time; }
	double get_jobs_current_process_time(int job_index) { return operation_seq[job_index][current_job_index[job_index]].duration; }
	int get_jobs_current_machine_id(int job_index) {return operation_seq[job_index][current_job_index[job_index]].machine_id; }
	void set_job_start_time(int job_index, double start_time) { operation_seq[job_index][current_job_index[job_index]].start_time = start_time; };
	double get_operation_finish_time();
	void test();
};

void test_JSSP_sol(const std::vector<int> &sol, int n_jobs, int n_machines);

#endif