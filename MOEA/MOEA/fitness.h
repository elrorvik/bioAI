#pragma once
#ifndef FITNESS_H
#define FITNESS_H
#include<vector>
struct RGB;
struct node;
struct pos;
class Population;

double dist(RGB p1, RGB p2);
double overall_deviation_seg(Population &p, int ind_index, pos s_entry);
double overall_deviation_ind(Population &p, int ind_index, const std::vector<pos> &entry_s);
double edge_value_seg(Population &p, int ind_index, pos s_entry);
double edge_value_ind(Population &p, int ind_index, const std::vector<pos> &entry_s);
RGB avg_rgb_seg(Population &p, int ind_index, pos s_entry);

#endif