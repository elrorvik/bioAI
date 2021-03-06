#pragma once

#ifndef GLOBAL_H
#define GLOBAL_H

#include<string>
#include<vector>
#include<map>

const std::string PATH_WRITE_PARETO_FRONT = "..//Draw_Pareto_Front//pareto_front.txt"; // 

const int CONTINUE_WHEN_DRAWING = 1; // For cv::WaitKey()
const int N_SEG_TYPES = 2;
const double CROSSOVER_RATE = 0.001;
const double MUTATION_RATE = 0.9;
const double MUT_SPLIT_PERC = 0.1;
const double MUT_MERGE_PERC = 0.9;
//const int SEGMENT_SIZE[N_SEG_TYPES] = { 1000,5000 };
//const int N_PIX_SEGMENT[N_SEG_TYPES] = { 20,20};


// dessert - COMPLETELY SATISFIED, do not touch!
/*
const int N_IND = 10;
const int N_OFFSPRING = 10;
const int N_GENERATIONS = 60; //100
const int SMALL_SEGMENT_SIZE = 100; // 20
const int N_SMALL_SEGMENT = 1000; // 3000
const int N_MERGE_SMALL_SEGMENT = 0; // 2990
const int AVG_RGB_SEG_DEST_CRITERIA = 70;
const int AVG_RGB_BORDER_DEST_CRITERIA = 70;
const std::string img_path = "..//test_img//178054//Test image.jpg"; // Desert
const std::string WRITE_PATH = "..//Segmentation_Evaluation//178054_student//"; // Desert
*/

// Circle - Completely satisfied, do not touch!
/*
const int N_IND = 20;
const int N_OFFSPRING = 10;
const int N_GENERATIONS = 60; // with 600 N_SMALL_SEGMENT -> 43 gives 69.73%
const int SMALL_SEGMENT_SIZE = 100; // 4, 100
const int N_SMALL_SEGMENT = 1000; // 600 -> 69.73%
const int N_MERGE_SMALL_SEGMENT = 0; // 1950
const int AVG_RGB_SEG_DEST_CRITERIA = 70;
const int AVG_RGB_BORDER_DEST_CRITERIA = 150;
const std::string img_path  = "..//test_img//86016//Test image.jpg"; // circle
const std::string WRITE_PATH = "..//Segmentation_Evaluation//86016_student//";
const bool MOEA_NOT_WGA = true;
const double WEIGHT_OD = 1;
const double WEIGHT_EV = 100000;*/


 // Tiger // N_GENERATIONS = 3, N_SMALL_SEGMENT = 300, SMALL_SEGMENT_SIZE = 400, AVG_RGB_SEG = 70, AVG_RGB_BORDER = 100 -> 67.64%
/*
const int N_IND = 10;
const int N_OFFSPRING = 10;
const int N_GENERATIONS = 10;
const int SMALL_SEGMENT_SIZE = 300;  // 50
const int N_SMALL_SEGMENT = 300; // 1000
const int N_MERGE_SMALL_SEGMENT = 0; // 30
const int AVG_RGB_SEG_DEST_CRITERIA = 70;
const int AVG_RGB_BORDER_DEST_CRITERIA = 100;
const std::string img_path = "..//test_img//160068//Test image.jpg"; 
const std::string WRITE_PATH = "..//Segmentation_Evaluation//160068_student//";
*/

// Rock in desert - COMPLETELY SATISFIED, do not touch!
/*
const int N_IND = 10;
const int N_OFFSPRING = 10;
const int N_GENERATIONS = 10;
const int SMALL_SEGMENT_SIZE = 500;  // 50
const int N_SMALL_SEGMENT = 300; // 1000
const int N_MERGE_SMALL_SEGMENT = 0; // 30
const int AVG_RGB_SEG_DEST_CRITERIA = 70;
const int AVG_RGB_BORDER_DEST_CRITERIA = 100;
const std::string img_path = "..//test_img//216066//Test image.jpg";
const std::string WRITE_PATH = "..//Segmentation_Evaluation//216066_student//";
*/

// Flower - COIMPLETELY SATISFIED, do not touch!
/*
const int N_IND = 10;
const int N_OFFSPRING = 10;
const int N_GENERATIONS = 10;
const int SMALL_SEGMENT_SIZE = 500;  // 50
const int N_SMALL_SEGMENT = 300; // 1000
const int N_MERGE_SMALL_SEGMENT = 0; // 30
const int AVG_RGB_SEG_DEST_CRITERIA = 70;
const int AVG_RGB_BORDER_DEST_CRITERIA = 100;
const std::string img_path = "..//test_img//353013//Test image.jpg";
const std::string WRITE_PATH = "..//Segmentation_Evaluation//353013_student//";
*/

// Tree - 88% - COIMPLETELY SATISFIED, do not touch!
/*
const int N_IND = 10;
const int N_OFFSPRING = 10;
const int N_GENERATIONS = 10;
const int SMALL_SEGMENT_SIZE = 500;  // 50
const int N_SMALL_SEGMENT = 300; // 1000
const int N_MERGE_SMALL_SEGMENT = 0; // 30
const int AVG_RGB_SEG_DEST_CRITERIA = 70;
const int AVG_RGB_BORDER_DEST_CRITERIA = 100;
const std::string img_path = "..//test_img//147091//Test image.jpg";
const std::string WRITE_PATH = "..//Segmentation_Evaluation//147091_student//";
*/

// Valley - COIMPLETELY SATISFIED, do not touch!
/*
const int N_IND = 10;
const int N_OFFSPRING = 10;
const int N_GENERATIONS = 10;
const int SMALL_SEGMENT_SIZE = 500;  // 50
const int N_SMALL_SEGMENT = 300; // 1000
const int N_MERGE_SMALL_SEGMENT = 0; // 30
const int AVG_RGB_SEG_DEST_CRITERIA = 70;
const int AVG_RGB_BORDER_DEST_CRITERIA = 100;
const std::string img_path = "..//test_img//176035//Test image.jpg";
const std::string WRITE_PATH = "..//Segmentation_Evaluation//176035_student//";
const bool MOEA_NOT_WGA = true;
const double WEIGHT_OD = 1;
const double WEIGHT_EV = 1;
*/

// Valley - weighted GA
/*
const int N_IND = 30;
const int N_OFFSPRING = 10;
const int N_GENERATIONS = 100;
const int SMALL_SEGMENT_SIZE = 100;  // 50
const int N_SMALL_SEGMENT = 400; // 1000
const int N_MERGE_SMALL_SEGMENT = 0; // 30
const int AVG_RGB_SEG_DEST_CRITERIA = 70;
const int AVG_RGB_BORDER_DEST_CRITERIA = 100;
const std::string img_path = "..//test_img//176035//Test image.jpg";
const std::string WRITE_PATH = "..//Segmentation_Evaluation//176035_student//";
const double WEIGHT_OD = 1000;
const double WEIGHT_EV = 1;
const bool MOEA_NOT_WGA = false;
//*/

// Circle - weighted GA
/*
const int N_IND = 20;
const int N_OFFSPRING = 10;
const int N_GENERATIONS = 150; // with 600 N_SMALL_SEGMENT -> 43 gives 69.73%
const int SMALL_SEGMENT_SIZE = 100; // 4, 100
const int N_SMALL_SEGMENT = 1000; // 600 -> 69.73%
const int N_MERGE_SMALL_SEGMENT = 0; // 1950
const int AVG_RGB_SEG_DEST_CRITERIA = 100;
const int AVG_RGB_BORDER_DEST_CRITERIA = 150;
const std::string img_path = "..//test_img//86016//Test image.jpg"; // circle
const std::string WRITE_PATH = "..//Segmentation_Evaluation//86016_student//";
const double WEIGHT_OD = 1;
const double WEIGHT_EV = 100;
const bool MOEA_NOT_WGA = false;
*/

// Tree - weighted GA

//const int N_IND = 30;
//const int N_OFFSPRING = 10;
//const int N_GENERATIONS = 100;
//const int SMALL_SEGMENT_SIZE = 100;  // 50
//const int N_SMALL_SEGMENT = 400; // 1000

//
const int N_IND = 10;
const int N_OFFSPRING = 10;
const int N_GENERATIONS = 10;
const int SMALL_SEGMENT_SIZE = 500;  // 50
const int N_SMALL_SEGMENT = 300; // 1000
const int N_MERGE_SMALL_SEGMENT = 0; // 30
const int AVG_RGB_SEG_DEST_CRITERIA = 70;
const int AVG_RGB_BORDER_DEST_CRITERIA = 100;


const std::string img_path = "..//test_img//Test Image//test image_1.jpg";
const std::string WRITE_PATH = "..//Segmentation_Evaluation//1_student//";
const double WEIGHT_OD = 1;
const double WEIGHT_EV = 0;
const bool MOEA_NOT_WGA = false;
//*/

//const std::string img_path = "..//test_img//147091//Test image.jpg"; // three
//const std::string WRITE_PATH = "..//Segmentation_Evaluation//147091_student//"; 
//const std::string img_path  = "..//test_img//160068//Test image.jpg"; // Leopart
//const std::string WRITE_PATH = "..//Segmentation_Evaluation//160068_student//"; 
//const std::string img_path  = "..//test_img//176035//Test image.jpg"; // Land
//const std::string WRITE_PATH = "..//Segmentation_Evaluation//176035_student//"; 
//const std::string img_path  = "..//test_img//216066//Test image.jpg"; // stein og �rken
//const std::string WRITE_PATH = "..//Segmentation_Evaluation//216066_student//"; 
//const std::string img_path  = "..//test_img//353013//Test image.jpg"; // blomst
//const std::string WRITE_PATH = "..//Segmentation_Evaluation//353013_student//"; // 





enum direction { SELF, UP, DOWN, LEFT, RIGHT };

struct RGB {
	unsigned short r;
	unsigned short g;
	unsigned short b;
	RGB() : r(-1), b(-1), g(-1) {};
	RGB(unsigned short r, unsigned short g, unsigned short b) : r(r), b(b), g(g) {};
	void operator=(RGB other);
};

RGB operator+(RGB a, RGB b);
RGB operator/(RGB numerator, double denominator);
bool operator==(RGB& left, RGB& right);

struct pos {
	unsigned short x;
	unsigned short y;
	pos() : x(0), y(0) {};
	pos(unsigned short x, unsigned short y) : x(x), y(y) {};
	pos(int x, int y) : x(static_cast<unsigned short>(x)), y(static_cast<unsigned short>(y)) {};
};

struct node {
	bool color = 0;
	bool up = 0;
	bool down = 0;
	bool left = 0;
	bool right = 0;
	direction parent_dir = SELF;
	unsigned int num_children = 0;
	pos entry = pos(-1, -1); // entry to segment
};

struct edge {
	pos p1;
	pos p2;
	double RGBdist = 0;
	int dependent_nodes = 0;
	edge(pos p1, pos p2, double RGBdist) : p1(p1), p2(p2), RGBdist(RGBdist) {};
	edge(pos p1, pos p2, double RGBdist, int n_nodes) : p1(p1), p2(p2), RGBdist(RGBdist), dependent_nodes(n_nodes) {};
};

struct edge_comparator {
	bool operator()(edge e1, edge e2) {
		return e1.RGBdist < e2.RGBdist;
	}
};

struct r_edge_comparator {
	bool operator()(edge e1, edge e2) {
		return e1.RGBdist < e2.RGBdist;
	}
};

struct pos_comparator {
	bool operator()(const pos &rhs) {
		return rhs.x == p1.x && rhs.y == p1.y;
	}
	pos_comparator(const pos p1) : p1(p1) {};
private:
	const pos p1;
};

struct pos_order_comparator {
	bool operator()(const pos &p1, const pos &p2) {
		if (p1.x < p2.x) return true;
		if (p1.x == p2.x) return (p1.y < p2.y);
		return false;
	}
	//pos_order_comparator(const pos p1) : p1(p1) {};
//private:
	//const pos p1;
};

struct edge_equal_comparator {
	bool operator()(const edge &rhs) {
		return rhs.p1.x == e1.p1.x && rhs.p1.y == e1.p1.y && rhs.p2.x == e1.p2.x && rhs.p2.y == e1.p2.y;
	}
	edge_equal_comparator(const edge e1) : e1(e1) {};
private:
	const edge e1;
};

struct active_edge_t {
	edge edge;
	bool active;
};



struct seg_prop_t {
	RGB avg_rgb;
	std::map<pos, std::vector<edge>> borders;
	std::vector<pos> neighbour_entries;
	seg_prop_t() :avg_rgb(-1, -1, -1) {};
};


void operator+(node &n, direction d);
void operator-(node &n, direction d);

pos operator+(pos coord, direction dir);
bool operator==(pos& left, pos & right);
bool operator!=(pos& left, pos & right);
pos operator+(pos& left, pos& right);
bool operator<(pos p1, pos p2);
bool operator==(edge e1, edge e2);

bool operator<(edge e1, edge e2);

direction get_neighbor_dir(pos& origin, pos& neighbor);


#endif 