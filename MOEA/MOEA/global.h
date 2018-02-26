#pragma once
#include<string>
#ifndef GLOBAL_H
#define GLOBAL_H

const int N_IND = 1;
const int N_SEG = 8;
const std::string img_path  = "..//test_img//1//Test image.jpg";

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
	pos entry; // entry to segment
};

pos operator+(pos coord, direction dir);

void operator+(node &n, direction d);
void operator-(node &n, direction d);

#endif 