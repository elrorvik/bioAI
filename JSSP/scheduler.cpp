#pragma once
#include<iostream>
#include<algorithm>
#include"scheduler.h"
#include"operation_manager.h"

void develop(Operation_manager& om, std::vector<int>& genotype) {
	om.reset_all_jobs();
	struct start_duration_pair {
		double start;
		double duration;
		start_duration_pair(double start, double duration) : start(start), duration(duration) {};
		bool operator<(const start_duration_pair& rhs) {
			return this->start < rhs.start;
		}
	};

	std::vector<std::vector<start_duration_pair>> machine_vacancies_tab;
	std::vector<double> job_finish_time(om.get_n_jobs(), 0);
	for (int m_index = 0; m_index < om.get_n_machines(); m_index++) {
		machine_vacancies_tab.emplace_back();
		machine_vacancies_tab[m_index].emplace_back(0, DBL_MAX);
	}
	
	// DEBUG :
	//std::vector<int> job_count(om.get_n_jobs(), 0);
	// : DEBUG
	
	for (int task_index = 0; task_index < genotype.size(); task_index++) {
		
		int task_job_ID = genotype[task_index];
		int task_machine_ID = om.get_jobs_current_machine_id(task_job_ID);
		
		
		double task_duration = om.get_jobs_current_process_time(genotype[task_index]);
		std::vector<start_duration_pair>* machine_vacancies = &machine_vacancies_tab[task_machine_ID];
	
		for (int vac_index = 0; vac_index < machine_vacancies->size(); vac_index++) {
			if ((*machine_vacancies)[vac_index].start + (*machine_vacancies)[vac_index].duration < job_finish_time[task_job_ID]) continue;

			double available_processing_time;
			double first_available_start_time;
			if ((*machine_vacancies)[vac_index].start > job_finish_time[task_job_ID]) {
				available_processing_time = (*machine_vacancies)[vac_index].duration;
				first_available_start_time = (*machine_vacancies)[vac_index].start;
			}
			else {
				available_processing_time = (*machine_vacancies)[vac_index].duration - (job_finish_time[task_job_ID] - (*machine_vacancies)[vac_index].start);
				first_available_start_time = job_finish_time[task_job_ID];
			}

			// DEBUG :
			//std::cout << task_duration << ", " << available_processing_time << std::endl;
			// : DEBUG

			if (task_duration <= available_processing_time) {

				// DEBUG :
				//std::cout << "Made it." << std::endl;
				// : DEBUG

				// Update job_finish_time
				job_finish_time[task_job_ID] = first_available_start_time + task_duration;

				// Update om
				om.set_job_start_time(task_job_ID, first_available_start_time);
				om.increment(task_job_ID);

				// Update vacancies
				if (first_available_start_time != (*machine_vacancies)[vac_index].start) { // Then create a new vacancy
					double new_vac_duration = first_available_start_time - (*machine_vacancies)[vac_index].start;
					(*machine_vacancies).emplace_back(first_available_start_time + task_duration, (*machine_vacancies)[vac_index].duration - new_vac_duration - task_duration);
					(*machine_vacancies)[vac_index].duration = new_vac_duration;
					//(*machine_vacancies)[vac_index].start unchanged
					std::sort((*machine_vacancies).begin(), (*machine_vacancies).end());
				}
				else {
					(*machine_vacancies)[vac_index].start += task_duration;
					if((*machine_vacancies)[vac_index].duration != DBL_MAX) (*machine_vacancies)[vac_index].duration -= task_duration;
				}

				if ((*machine_vacancies)[vac_index].duration <= task_duration / 10000000) {
					(*machine_vacancies).erase((*machine_vacancies).begin() + vac_index);
				}

				// DEBUG :
				//job_count[task_job_ID]++;
				// : DEBUG

				break; // Task inserted successfully, move on to next task
			}
		}
	}

	// DEBUG:
	for (int i = 0; i < om.get_n_machines(); i++) {
		if (om.get_current_job_index(i) != om.get_n_machines()) {
			std::cout << "current_job_index of job " << i << " is " << om.get_current_job_index(i) << ", should be " << om.get_n_machines() << std::endl;
		}
		//if (job_count[i] != om.get_n_machines()) {
		//	std::cout << "job " << i << " has not been counted " << om.get_n_machines() << " times" << std::endl;
		//}
	}
	// : DEBUG
	om.reset_increment();

	return;
}

double develop_makespan(Operation_manager& om, std::vector<int>& genotype) { // Changes genotype
	develop(om, genotype);
	return om.get_operation_finish_time();
}

double calc_makespan(Operation_manager& om, const std::vector<int>& genotype) { // Does not change genotype
	std::vector<int> genotype_copy = genotype;
	develop(om, genotype_copy);
	return om.get_operation_finish_time();
}

void print_schedule(Operation_manager& om) {
	return;
}