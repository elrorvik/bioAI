#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "operation_manager.h"

void read_file(std::string filename, Operation_manager& om,  int &n_jobs, int &n_machines, int ) {
	std::fstream f(filename);
	if (!f.is_open()) {
		std::cout << "Could not find file path" << std::endl;
		return;
	}

	/*std::string line;
	std::stringstream ss_line;
	std::getline(f, line);
	ss_line << line;
	ss_line >> n_jobs;
	ss_line >> n_machines;
	ss_line.str(std::string());
	ss_line.clear();


	int machine_id;
	double process_time;
	int taskId;
	for (int job_id = 0; job_id < n_jobs; ++job_id) {
		std::getline(f, line);
		ss_line << line;
		taskId = 0;
		while (!ss_line.eof()) {
			ss_line >> machine_id;
			ss_line >> process_time;
			jobs[job_id].push_back(Task(machine_id, job_id, taskId, process_time));
			taskId++;
		}
		ss_line.str(string());
		ss_line.clear();
	}
	current_job_index = vector<char>(numJobs, 0);*/

}