#include <iostream>
#include <math.h>
#include "global.h"
#include "fitness.h"



double dist(RGB p1, RGB p2) {
	return sqrt(pow(p1.r - p2.r, 2) + pow(p1.g - p2.g, 2) + pow(p1.b - p2.b, 2));
}

void find_pixels_segment(int x, int y) {

}