
#include <string>
#include <iostream>
#include"file.h"
#include "operation_manager.h"

Operation_manager::Operation_manager(std::string filename) {

	read_file(filename, this->n_jobs, this->n_machines, this->operation_seq);
	current_job_index = std::vector<int>(n_jobs, 0);
	//print_operation_sequence();
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
