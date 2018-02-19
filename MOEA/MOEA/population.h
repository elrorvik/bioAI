#pragma once
#include<vector>

struct RGB {
	int R;
	int G;
	int B;
};

enum node { self, up, down, left, right };

class Population
{
private:
	RGB **image;
	node ***population;
public:
	Population() {};
	~Population();
	void MST_Kruskal();
};