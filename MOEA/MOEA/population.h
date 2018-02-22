#pragma once

#ifndef POPULATION_H
#define POPULATION_H
#include<vector>
#include <opencv2/highgui/highgui.hpp>

struct pos;
struct node;
struct RGB;

class Population{
private:
	cv::Mat im;
	node ***population;
	std::vector<pos> * segment_nr;
	int n_segments;
public:
	Population();
	~Population();
	node** get_individual(int ind_index);
	void initialize_population_PrimsMST();
	RGB get_RGB(int y, int x);
	int get_im_w();
	int get_im_h();
	pos get_pixel_segment(int x, int y, int individual);
};

#endif