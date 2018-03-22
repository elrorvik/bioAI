#include <iostream>
#include <fstream>
#include <string>

void read_file(std::string filename) {
	std::fstream f(filename);
	if (!f.is_open()) {
		std::cout << "Could not find file path" << std::endl;
		return;
	}

	std::string line;
	/*std::stringstream ss_line;
	std::getline(f, line);
	ss_line << line;
	ss_line >> numJobs;
	ss_line >> numMachines;
	ss_line.str(string());
	ss_line.clear();


	jobs = schedule_t(numJobs, vector<Task>());
	int machine_id;
	float process_time;
	int taskId;
	for (int job_id = 0; job_id < numJobs; ++job_id) {
		getline(file, line);
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