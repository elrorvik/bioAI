#pragma once
#include <vector>
#include <string>

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
	std::vector<int> current_operation_index;
public:
	Operation_manager(std::string file_name);
	void print_operation_sequence();
};

