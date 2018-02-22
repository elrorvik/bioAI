#pragma once
#include<vector>

struct RGB {
	int R;
	int G;
	int B;
};

struct node {
	bool colored = 0;

	bool up = 0;
	bool down = 0;
	bool left = 0;
	bool right = 0;
};

class Population
{
private:
	RGB **image;
	node ***population;
public:
	Population() {};
	~Population();
	Population(int res_height, int res_width, int **pixels);
	void initialize_k_means();
};