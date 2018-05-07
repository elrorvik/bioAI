#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "operation_manager.h"

void read_file(std::string filename,  int &n_jobs, int &n_machines, operation_seq_t& operation_seq ) {
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
		ss<< line;
		int operation_id = 0;
		std::cout << job_id << std::endl;
		while (ss >> machine_id) {
			ss >> duration;
			operation_seq[job_id].push_back(operation_t(machine_id, operation_id, duration));
			operation_id++;
		}
		ss.clear();
	}
	f.close();
}

void write_file(std::string filename, Operation_manager& om) {
	std::fstream f(filename, std::ios::out);
	if (!f.is_open()) {
		std::cout << "Could not find file path" << std::endl;
		return;
	}
	om.reset_increment();
	
	f << om.get_n_machines();
	f << ",";
	f << om.get_n_jobs();
	f << ",";
	f << om.get_operation_finish_time();
	f << std::endl;

	for (int i = 0; i < om.get_n_jobs(); i++) {
		for (int line = 0; line < om.get_n_machines(); line++) {
			f << om.get_jobs_current_machine_id(i);
			f << ",";
			f << i;
			f << ",";
			f << om.get_jobs_current_start_time(i);
			f << ",";
			f << om.get_jobs_current_start_time(i) + om.get_jobs_current_process_time(i);
			f << std::endl;
			om.increment(i);
		}
	}
	
	f.close();
}