#pragma once
#ifndef JSSP_H
#define JSSP_H
#include<vector>

class Operation_manager;
void mutate(std::vector<int> &sol);
bool mutate_search(Operation_manager& om, std::vector<int> &sol, int sign);
void mutate_search_greedy(Operation_manager& om, std::vector<int> &sol, int sign);
void search_greedy(Operation_manager& om, std::vector<int> &sol, int sign, int start, int end);
#endif