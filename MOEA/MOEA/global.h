#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

const int N_IND = 30;
const std::string img_path  = "..//test_img//1//Test image.jpg";


struct RGB {
	int r;
	int g;
	int b;
	RGB(int r, int g, int b) : r(r), b(b), g(g) {};
};

struct pos {
	int x;
	int y;
};

struct node {
	bool colored = 0;

	bool up = 0;
	bool down = 0;
	bool left = 0;
	bool right = 0;
	pos entry; // entry to segment
};

#endif 