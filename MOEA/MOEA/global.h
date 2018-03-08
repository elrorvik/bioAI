#pragma once
#include<string>
#ifndef GLOBAL_H
#define GLOBAL_H


const int N_GENERATIONS = 40; //100
const int N_IND = 40;
const int N_SEG_TYPES = 2;

const double CROSSOVER_RATE = 0.6;
const double MUTATION_RATE = 0.9;
const int N_OFFSPRING = 10;
const double MUT_SPLIT_PERC = 0.1;
const double MUT_MERGE_PERC = 0.9;
//const int SEGMENT_SIZE[N_SEG_TYPES] = { 1000,5000 };
//const int N_PIX_SEGMENT[N_SEG_TYPES] = { 20,20};
const int SEGMENT_SIZE[N_SEG_TYPES] = { 1000,5000 };
const int N_PIX_SEGMENT[N_SEG_TYPES] = { 10,1};
const int N_EDGES = 60;
//const int SEGMENT_SIZE[N_SEG_TYPES] = { 1000,5000 };
//const int N_PIX_SEGMENT[N_SEG_TYPES] = { 3,3 };

/*
//const int N_GENERATIONS = 100;
const int N_IND = 10;
const int N_SEG_TYPES = 2;
const double CROSSOVER_RATE = 0.5;
const double MUTATION_RATE = 0.5;
const int N_OFFSPRING = 6;
const double MUT_SPLIT_PERC = 0.1;
const double MUT_MERGE_PERC = 0.9;

//Desert Circle
const int N_GENERATIONS = 200;
const int SEGMENT_SIZE[N_SEG_TYPES] = { 1000,10000 };
const int N_PIX_SEGMENT[N_SEG_TYPES] = { 60,20 };
const std::string img_path = "..//test_img//86016//Test image.jpg";
const std::string WRITE_PATH = "..//Segmentation_Evaluation//86016_student//";

//Flowers, low amount of starting segments
//const int SEGMENT_SIZE[N_SEG_TYPES] = { 1000,10000 };
//const int N_PIX_SEGMENT[N_SEG_TYPES] = { 60,20 };
//const std::string img_path  = "..//test_img//353013//Test image.jpg"; // blomst
//const std::string WRITE_PATH = "..//Segmentation_Evaluation//353013_student//";
*/

//const std::string img_path  = "..//test_img//86016//Test image.jpg"; // circle
//const std::string WRITE_PATH = "..//Segmentation_Evaluation//86016_student//"; // �rken
//const std::string img_path = "..//test_img//147091//Test image.jpg"; // three
//const std::string img_path  = "..//test_img//160068//Test image.jpg"; // Leopart
//const std::string WRITE_PATH = "..//Segmentation_Evaluation//160068_student//"; 
//const std::string img_path  = "..//test_img//176035//Test image.jpg"; // Land
const std::string img_path  = "..//test_img//178054//Test image.jpg"; // �rken
const std::string WRITE_PATH = "..//Segmentation_Evaluation//178054_student//"; // �rken
//const std::string img_path  = "..//test_img//216066//Test image.jpg"; // stein og �rken
//const std::string img_path  = "..//test_img//353013//Test image.jpg"; // blomst
//const std::string WRITE_PATH = "..//Segmentation_Evaluation//353013_student//"; // �rken
//const std::string WRITE_PATH = "..//Segmentation_Evaluation//353013_student//";

//const std::string img_path = "..//2000px-Checkerboard_pattern_2.png";
//const std::string img_path = "..//test_2.jpg";
//const std::string img_path = "..//test_3.png";



enum direction { SELF, UP, DOWN, LEFT, RIGHT };

struct RGB {
	unsigned short r;
	unsigned short g;
	unsigned short b;
	RGB(unsigned short r, unsigned short g, unsigned short b) : r(r), b(b), g(g) {};
};

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

struct active_edge_t {
	edge edge;
	bool active;
};

pos operator+(pos coord, direction dir);

void operator+(node &n, direction d);
void operator-(node &n, direction d);

bool operator==(pos& left, pos & right); 

bool operator!=(pos& left, pos & right);
pos operator+(pos& left, pos& right);

bool operator<(edge e1, edge e2);

#endif 