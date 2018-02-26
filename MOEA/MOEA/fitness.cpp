#include <iostream>
#include <math.h>
#include "global.h"
#include "fitness.h"
#include "population.h"
#include "graph.h"



double dist(RGB p1, RGB p2) {
	return sqrt(pow(p1.r - p2.r, 2) + pow(p1.g - p2.g, 2) + pow(p1.b - p2.b, 2));
}

double overall_deviation(Population &p, int ind_index, node s_entry) {
	std::stack<pos> branch_points;
	pos pos_i;
	pos_i = traverse_ST(p, ind_index, s_entry.entry, branch_points);

}



void find_pixels_segment(int x, int y) {
	
}
