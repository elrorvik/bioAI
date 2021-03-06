#pragma once
#ifndef VAR_OPERATORS
#define VAR_OPERATORS

#include <vector>

class Population;
struct active_edge_t;

std::vector<active_edge_t> crossover_uniform_list_representation(Population &p, int parent_A, int parent_B, int offspring_index);
int crossover_merge(Population &p, int parent_index, int offspring_index);
int mutation_split_segments(Population &p, int ind_index);
int mutation_merge_segments(Population &p, int ind_index);
int mutation_greedy_merge_segments(Population &p, int ind_index);

#endif