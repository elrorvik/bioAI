#pragma once
#ifndef NSGAII_H
#define NSGAII_H
#include<vector>

struct Population;
struct pos;
std::vector<int> NSGAII(Population &p, const std::vector<pos> * entry_s, int n_pop);

#endif