#pragma once
#include <vector>

struct operation_t {
	double start_time;
	const double duration;
	const int machine_id;
	const int operation_id;
	operation_t(double duration, int machine_id, int task_id) : start_time(-1.0), duration(duration), machine_id(machine_id), task_id(task_id) {};
	operation_t(double start_time,double duration, int machine_id, int task_id) : start_time(start_time), duration(duration), machine_id(machine_id), task_id(task_id) {};
};

typedef std::vector<std::vector<operation_t>> operation_seq_t;

class Operation_manager
{
private:
	int n_machines;
	int n_jobs;
	operation_seq_t operation_seq;
	std::vector<int> current_operation_index;
public:
	Operation_manager();
	~Operation_manager();

	void set_n_machines(int n_machines) { this->n_machines = n_machines; };
	void set_n_jobs(int n_jobs) { this->n_jobs = n_jobs; };


};

