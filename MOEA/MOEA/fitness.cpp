#include <iostream>
#include <math.h>
#include "global.h"
#include "fitness.h"
#include "population.h"
#include "graph.h"

double dist(RGB p1, RGB p2) {
	return sqrt(pow(p1.r - p2.r, 2) + pow(p1.g - p2.g, 2) + pow(p1.b - p2.b, 2));
}

double overall_deviation_seg(Population &p, int ind_index, pos s_entry) {

	// Calculate centroid
	std::stack<pos> branch_points;
	pos pos_i = s_entry;
	int num_nodes_in_segment = 1;
	int sum_R = 0;
	int sum_G = 0;
	int sum_B = 0;
	while (pos_i.x != static_cast<unsigned short>(-1)) {
		sum_R += p.get_RGB(pos_i).r;
		sum_G += p.get_RGB(pos_i).g;
		sum_B += p.get_RGB(pos_i).b;
		num_nodes_in_segment++;
		pos_i = traverse_ST(p, ind_index, s_entry, branch_points);
	}

	RGB centroid(sum_R / num_nodes_in_segment, sum_G / num_nodes_in_segment, sum_B / num_nodes_in_segment);
	double fitness = 0;
	pos_i = s_entry;
	while (pos_i.x != static_cast<unsigned short>(-1)) {
		fitness = dist(p.get_RGB(pos_i), centroid);
		pos_i = traverse_ST(p, ind_index, s_entry, branch_points);
	}

	return fitness;
}

double overall_deviation_ind(Population &p, int ind_index, const std::vector<pos> &entry_s) {
	// Testing:
	// static int i = 0;
	// double test_list[11] = { 1.1, 3.7, 7.1, 0.3, 4.5, 9.9, 4.3, 4.8, 8.3, 10.3, 0.1 };
	// return test_list[i++];*/

	double fitness = 0;
	for (int seg_i = 0; seg_i < entry_s.size(); seg_i++) {
		fitness += overall_deviation_seg(p, ind_index, entry_s[seg_i]);
	}
	return fitness;
}

double edge_value_seg(Population &p, int ind_index, pos s_entry) {

	std::stack<pos> branch_points;
	pos pos_i = s_entry;
	double fitness = 0;

	while (pos_i.x != static_cast<unsigned short>(-1)) {
		node *node_i = p.get_node(ind_index, pos_i);

		if (pos_i.y > 0) {
			node *node_j = p.get_node(ind_index, pos_i + UP);
			if (node_i->entry.x != node_j->entry.x || node_i->entry.y != node_j->entry.y) { // Replace with proper func
				fitness += dist(p.get_RGB(pos_i), p.get_RGB(pos_i + UP));
			}
		}
		if (pos_i.y < p.get_im_h()) {
			node *node_j = p.get_node(ind_index, pos_i + DOWN);
			if (node_i->entry.x != node_j->entry.x || node_i->entry.y != node_j->entry.y) {
				fitness += dist(p.get_RGB(pos_i), p.get_RGB(pos_i + DOWN));
			}
		}
		if (pos_i.x > 0) {
			node *node_j = p.get_node(ind_index, pos_i + LEFT);
			if (node_i->entry.x != node_j->entry.x || node_i->entry.y != node_j->entry.y) {
				fitness += dist(p.get_RGB(pos_i), p.get_RGB(pos_i + LEFT));
			}
		}
		if (pos_i.x < p.get_im_w()) {
			node *node_j = p.get_node(ind_index, pos_i + RIGHT);
			if (node_i->entry.x != node_j->entry.x || node_i->entry.y != node_j->entry.y) {
				fitness += dist(p.get_RGB(pos_i), p.get_RGB(pos_i + RIGHT));
			}
		}

		pos_i = traverse_ST(p, ind_index, s_entry, branch_points);
	}

	return -fitness; // fitness defined negative in order to turn a maximation problem into a minimation problem
}

double edge_value_ind(Population &p, int ind_index, const std::vector<pos> &entry_s) {
	// Testing
	// static int i = 0;
	// double test_list[11] = { -1.3, -3.2, -7.0, -0.9, -4.9, -9.2, -4.0, -13.0, -8.8, -10.9, -20.0 };
	// return test_list[i++];

	/*double fitness = 0;
	for (int seg_i = 0; seg_i < entry_s.size(); seg_i++) {
	fitness += edge_value_seg(p, ind_index, entry_s[seg_i]);
	}
	return fitness;*/
}

