#include<iostream>
#include"scheduler.h"
#include"operation_manager.h"

void develop(Operation_manager& om, std::vector<int>& genotype) {

	struct start_duration_pair {
		double start;
		double duration;
		start_duration_pair(double start, double duration) : start(start), duration(duration) {};
		bool operator()(const start_duration_pair& lhs, const start_duration_pair& rhs) {
			return lhs.start < rhs.start;
		}
	};

	std::vector<std::vector<start_duration_pair>> machine_vacancies_tab;
	std::vector<double> job_finish_time(om.get_n_jobs(), 0);
	for (int m_index = 0; m_index < om.get_n_machines(); m_index++) {
		machine_vacancies_tab.emplace_back();
		machine_vacancies_tab[m_index].emplace_back(0, DBL_MAX);
	}

	// DEBUG:
	std::cout << "Here" << std::endl;
	// : DEBUG

	for (int task_index = 0; task_index < genotype.size(); task_index++) {
		int task_job_ID = genotype[task_index];
		int task_machine_ID = om.get_jobs_current_machine_id(task_job_ID);
		double task_duration = om.get_jobs_current_process_time(genotype[task_index]);
		std::vector<start_duration_pair>* machine_vacancies = &machine_vacancies_tab[task_machine_ID];

		for (int vac_index = 0; vac_index < machine_vacancies->size(); vac_index++) {
			double available_duration;
			double prev_jobs_finish_time;

			if (om.get_current_job_index(task_job_ID) == 0) prev_jobs_finish_time = 0;
			else prev_jobs_finish_time = om.get_jobs_current_start_time(task_job_ID - 1) + om.get_jobs_current_process_time(task_job_ID - 1);

			if ((*machine_vacancies)[vac_index].start > job_finish_time[task_job_ID]) {
				available_duration = (*machine_vacancies)[vac_index].duration;
			}
			else {
				available_duration = (*machine_vacancies)[vac_index].duration - (prev_jobs_finish_time - (*machine_vacancies)[vac_index].start);
			}

			if (task_duration <= available_duration) {
				om.set_job_start_time(task_job_ID, (*machine_vacancies)[vac_index].start);
				om.increment(task_job_ID);

				(*machine_vacancies)[vac_index].start += task_duration;
				if (vac_index < (*machine_vacancies).size() - 1) { // Last vacancy should always have infinite duration - we can always add tasks to the last vacancy
					(*machine_vacancies)[vac_index].duration -= task_duration;
				}

				if ((*machine_vacancies)[vac_index].duration <= 0) {
					(*machine_vacancies).erase((*machine_vacancies).begin() + vac_index);
					vac_index--;
				}
			}
		}
		// DEBUG:
		for (int i = 0; i < om.get_n_jobs(); i++) {
			if (om.get_current_job_index(i) != om.get_n_jobs()) {
				std::cout << "current_job_index of job " << i << " is " << om.get_current_job_index(i) << ", should be " << om.get_n_jobs() << std::endl;
			}
		}
		// : DEBUG
		om.reset_increment();
	}

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