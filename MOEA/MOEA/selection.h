#pragma once
#ifndef SELECTION
#define SELECTION
#include<vector>

class Population;
struct pos;
std::vector<int> NSGAII(Population &p, const std::vector<pos> * entry_s, int n_pop, std::vector<std::pair<int, int>> rank_individuals, const std::vector<std::pair<double, int>> &fitness_1, const std::vector<std::pair<double, int>> fitness_2);
void MOEA_fitness(Population &p, int n_pop, const std::vector<pos> * entry_s, std::vector<std::pair<double, int>> &fitness_1, std::vector<std::pair<double, int>> &fitness_2);
void MOEA_rank(int n_pop, std::vector<std::pair<int, int>> &rank_individuals, const std::vector<std::pair<double, int>> &fitness_1, const std::vector<std::pair<double, int>> &fitness_2);
void MOEA_fitness_range(int n_pop, const std::vector<std::pair<double, int>> &fitness_1, const std::vector<std::pair<double, int>> &fitness_2, double &f_1_max, double &f_2_max, double &f_1_min, double &f_2_min);
std::vector<int> rank_tournament_selection(Population &p, const std::vector<pos> * entry_s, int n_pop, int tournament_size, int num_chosen, const std::vector<std::pair<int, int>> &rank_individuals);
std::vector<int> fitness_tournament_selection(Population &p, const std::vector<pos> * entry_s, int n_pop, int tournament_size, int num_chosen, const std::vector<std::pair<double, int>> &fitness_1, const std::vector<std::pair<double, int>> &fitness_2, double weight_1, double weight_2);
#endif