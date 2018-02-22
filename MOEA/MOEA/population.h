#pragma once
#include<vector>

struct RGB {
	int R;
	int G;
	int B;
};

enum node { SELF, UP, DOWN, RIGHT, LEFT };

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