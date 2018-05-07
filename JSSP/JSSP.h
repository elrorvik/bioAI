#pragma once
#ifndef JSSP_H
#define JSSP_H
#include<vector>

class Operation_manager;
void mutate(std::vector<int> &sol);
void mutate_search(Operation_manager& om, std::vector<int> &sol, int sign);
void mutate_search_greedy(Operation_manager& om, std::vector<int> &sol, int sign);
void search_greedy(Operation_manager& om, std::vector<int> &sol, int sign, int start, int end);
bool end_condition_and_summary(Operation_manager& om, int iteration, int cout_period, double target);
#endif