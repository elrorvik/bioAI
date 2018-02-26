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
	std::vector<pos> * entry_s;
	int n_segments;
public:
	Population();
	~Population();
	//node** get_individual(int ind_index);
	node* get_node(int ind_index, pos node_pos);
	void initialize_population_PrimsMST();
	void initialize_population_PrimsMST_2();
	RGB get_RGB(int y, int x);
	RGB get_RGB(pos pixel_pos);
	int get_im_w();
	int get_im_h();
	pos get_pixel_segment(int x, int y, int individual);
	void initialize_population_test();
};

#endif