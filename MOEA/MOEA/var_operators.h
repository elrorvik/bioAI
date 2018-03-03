#pragma once
#ifndef VAR_OPERATORS
#define VAR_OPERATORS
#include <vector>

class Population;
struct active_edge_t;

std::vector<active_edge_t> crossover_uniform_list_representation(Population &p, int parent_A, int parent_B);
int  mutation_split_segments(Population &p, int ind_index);
int  mutation_merge_segments(Population &p, int ind_index);
// Variaton operators (?)
// GA

#endif