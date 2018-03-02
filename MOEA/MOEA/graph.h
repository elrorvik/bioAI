#pragma once
#include <list>
#include <vector>
#include <stack>
using namespace std;

# define INF 0x3f3f3f3f

// iPair ==>  Integer Pair
typedef pair<int,double> idPair;
typedef pair<double, int> diPair;

// This class represents a directed graph using
// adjacency list representation
class Graph
{
	int V;    // No. of vertices
	list<idPair> *adj; //  adjencency list

public:
	Graph(int V);  // Constructor
	~Graph();
	void addEdge(int u, int v, double w);  // function to add an edge to graph
	vector<int> primMST(); // creating MST returning parents
};

class Population;
struct pos;
pos traverse_ST(Population &p, int ind_index, pos entry, stack<pos> &branch_points);
void remove_color(Population &p, int ind_index, pos entry, std::stack<pos> &branch_points);
void set_num_children(Population &p, int ind_index, pos entry);