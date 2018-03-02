#pragma once

#ifndef POPULATION_H
#define POPULATION_H
#include<vector>
#include <opencv2/highgui/highgui.hpp>
#include <queue>
#include <map>

struct pos;
struct node;
struct RGB;
struct edge;
struct edge_comparator;

typedef std::priority_queue<edge, std::vector<edge>, edge_comparator> edge_priority_que;

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
	void draw_segments(int ind_index);
	void draw_segments_contour(int ind_index);
	int get_n_segment(pos& entry, int ind_index);
	int check_if_edge(pos curr, int ind_index);

	int set_segment_value(pos& entry, int ind_index);
	void set_dir_edge(pos& parent, pos& child, int on, int ind_index);

	void test_segment(pos& entry, int ind_index);
	std::vector<pos>* edges_segment(int ind_index);
	void create_segments(int ind_index, int segment_size, edge_priority_que& que, int n_segments);
	void initialize_n_children(int ind_index, std::map<edge, int> &edgeChildren);
	int get_n_dependent_children(pos curr, pos next, int ind_index);
	void change_parents_n_segment(pos& parent, pos& child, int ind_index);
};

cv::Mat test_image();

#endif