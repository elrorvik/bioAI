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
struct r_edge_comparator;
struct active_edge_t;
struct seg_prop_t;

typedef std::priority_queue<edge, std::vector<edge>, edge_comparator> edge_priority_que;
typedef std::priority_queue<edge, std::vector<edge>, r_edge_comparator> r_edge_priority_que;



class Population{
private:
	cv::Mat im;
	node ***population;
	std::vector<std::pair<int, int>> rank;
	std::vector<std::pair<double, int>> fitness_1;
	std::vector<std::pair<double, int>> fitness_2;
	std::vector<pos> * entry_s;
	int n_segments;
	int n_pop;
	std::vector<active_edge_t> *edge_candidates;
	
	std::map<pos, seg_prop_t>* segment_prop;

public:
	Population();
	~Population();
	//node** get_individual(int ind_index);
	//void initialize_population_PrimsMST();
	void initialize_individual_PrimsMST(int ind_index);
	void initialize_population();
	//void initialize_population_test();
	void test_prim();
	void test_segment(pos& entry, int ind_index);

	void MOEA_next_generation();

	node* get_node(int ind_index, pos node_pos);
	RGB get_RGB(int y, int x);
	RGB get_RGB(pos pixel_pos);
	int get_im_w();
	int get_im_h();
	int get_parent_segment_size(pos& parent, int ind_index);
	pos get_pixel_segment(int x, int y, int individual);
	int get_n_segment(pos& entry, int ind_index, bool check_for_taken);
	std::vector<pos> get_segment_entries(int ind_index);
	seg_prop_t get_segment_property(int ind_index, pos seg_entry);

	bool get_pos_within_borders(pos& p);

	std::vector<active_edge_t>& get_edge_candidates(int ind_index);

	void draw_segments(int ind_index);
	void draw_segments_contour(int ind_index, int name);
	cv::Mat draw_segments_black_contour(int ind_index);
	std::vector<pos>* edges_segment(int ind_index);
	
	//int check_if_edge(pos curr, int ind_index);
	int check_if_edge(pos curr, int ind_index, int cout); // checkk if boarder

	void merge_segments(int ind_index, edge merge_nodes);
	void merge_segments(int ind_index, int edge_index, edge merge_nodes);
	int merge_segment_properties(int ind_index, pos first, pos second);
	void split_segment(int ind_index, int edge_index, edge split_nodes);

	void set_dir_edge(pos& parent, pos& child, int on, int ind_index);
	void set_dir_edge_and_parent(pos& parent, pos& child, int on, int ind_index);
	int set_start_segment_entry(pos& entry, int ind_index);
	int set_segment_entry(pos& entry, pos& set, int ind_index);


	int split_MST_segment(int ind_index,  edge_priority_que& que, r_edge_priority_que& merge_que);
	int merge_small_segments(int ind_index, r_edge_priority_que& que, int n_segments);
	void change_parents_n_segment(pos& parent, pos& child, int ind_index);
	std::vector<edge> get_neigbours(pos curr, int ind_index, int cout);

	//void initialize_n_children(int ind_index, std::map<edge, int> &edgeChildren);
	//int get_n_dependent_children(pos curr, pos next, int ind_index);

	void copy_individual(int l_index, int r_index);
	void draw_pareto_front();
};

cv::Mat test_image();

#endif