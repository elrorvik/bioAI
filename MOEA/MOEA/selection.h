#pragma once
#ifndef SELECTION
#define SELECTION
#include<vector>

struct Population;
struct pos;
std::vector<int> NSGAII(Population &p, const std::vector<pos> * entry_s, int n_pop);
void MOEA_rank(Population &p, const std::vector<pos> * entry_s, int n_pop, std::vector<std::pair<int, int>> &rank_individuals, std::vector<std::pair<double, int>> &fitness_1, std::vector<std::pair<double, int>> &fitness_2, double &f_1_max, double &f_2_max, double &f_1_min, double &f_2_min);
void MOEA_rank(Population &p, const std::vector<pos> * entry_s, int n_pop, std::vector<std::pair<int, int>> &rank_individuals);
std::vector<int> rank_tournament_selection(Population &p, const std::vector<pos> * entry_s, int n_pop, int tournament_size, int num_chosen);
std::vector<int> fitness_tournament_selection(Population &p, const std::vector<pos> * entry_s, int n_pop, int tournament_size, int num_chosen, double weight_1, double weight_2);
#endif