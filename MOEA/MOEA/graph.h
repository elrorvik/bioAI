#pragma once
#include <list>
#include <vector>
#include <stack>
using namespace std;

# define INF 0x3f3f3f3f

//typedef pair<int,double> idPair;
typedef pair<double, int> diPair;

class Graph
{
	int V;    
	list<diPair> *adj; 

public:
	Graph(int V);  
	~Graph();
	void addEdge(int u, int v, double w);  
	vector<int> primMST(int start_index); 
};

class Population;
struct pos;
pos traverse_ST(Population &p, int ind_index, pos entry, stack<pos> &branch_points);
void remove_color(Population &p, int ind_index, pos entry, std::stack<pos> &branch_points);
void set_num_children(Population &p, int ind_index, pos entry);