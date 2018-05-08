#pragma once
#ifndef SCHEDULER_H
#define SCHEDULER_H
#include<vector>

class Operation_manager;

void develop(Operation_manager& om, std::vector<int>& genotype);
void develop_greedy(Operation_manager& om, std::vector<int>& genotype);
double develop_makespan(Operation_manager& om, std::vector<int>& genotype);
double develop_greedy_makespan(Operation_manager& om, std::vector<int>& genotype);
double calc_makespan(Operation_manager& om, const std::vector<int>& genotype);
void print_schedule(Operation_manager& om);

#endif