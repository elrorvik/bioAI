#include "graph.h"
#include "population.h"
#include "global.h"
#include <vector>
#include <list>
#include<iostream>
#include<queue>
#include <list>
#include <time.h>
#include <functional>


using namespace std;


// Allocates memory for adjacency list
Graph::Graph(int V){
	this->V = V;
	adj = new list<idPair>[V];
}

Graph::~Graph(){
	delete[] adj;
}

void Graph::addEdge(int u, int v, double w){
	adj[u].push_back(make_pair(v, w));
	adj[v].push_back(make_pair(u, w));
}


vector<int> Graph::primMST(){
	// Create a priority queue to store vertices that
	// are being preinMST. This is weird syntax in C++.
	// Refer below link for details of this syntax
	// http://geeksquiz.com/implement-min-heap-using-stl/
	priority_queue< diPair, vector <diPair>, greater<diPair> > pq;

	int src = 0; // Taking vertex 0 as source

				 // Create a vector for keys and initialize all
				 // keys as infinite (INF)
	vector<double> key(V, INF);

	// To store parent array which in turn store MST
	vector<int> parent(V, -1);

	// To keep track of vertices included in MST
	vector<bool> inMST(V, false);

	// Insert source itself in priority queue and initialize
	// its key as 0.
	pq.push(make_pair(0.0, src));
	key[src] = 0.0;

	/* Looping till priority queue becomes empty */
	while (!pq.empty())
	{
		// The first vertex in pair is the minimum key
		// vertex, extract it from priority queue.
		// vertex label is stored in second of pair (it
		// has to be done this way to keep the vertices
		// sorted key (key must be first item
		// in pair)
		int u = pq.top().second;
		pq.pop();

		inMST[u] = true;  // Include vertex in MST

						  // 'i' is used to get all adjacent vertices of a vertex
		list< pair<int, double> >::iterator i;
		for (i = adj[u].begin(); i != adj[u].end(); ++i)
		{
			// Get vertex label and weight of current adjacent
			// of u.
			int v = (*i).first;
			double weight = (*i).second;

			//  If v is not in MST and weight of (u,v) is smaller
			// than current key of v
			if (inMST[v] == false && key[v] > weight)
			{
				// Updating key of v
				key[v] = weight;
				pq.push(make_pair(key[v], v));
				parent[v] = u;
			}
		}
	}

	// Print edges of MST using parent array
	/*for (int i = 1; i < V; ++i)
	printf("%d - %d\n", parent[i], i);*/
	return parent;
}

pos traverse_ST(Population &p, int ind_index, pos entry, std::stack<pos> &branch_points) {
	node* current = p.get_node(ind_index, entry);
	current->color = !current->color;

	int num_added = 0;
	pos next_entry;
	node* next = NULL;
	if (current->up == 1) {
		next = p.get_node(ind_index, entry + UP);
		/*std::cout << "up ";
		std::cout << current->up << std::endl;
		std::cin.get();*/
		if (next->color == !current->color) {
			num_added++;
			next_entry = entry + UP;
		}
	}
	if (current->down == 1) {
		next = p.get_node(ind_index, entry + DOWN);
		/*std::cout << "down ";
		std::cout << current->down << std::endl;
		std::cin.get();*/
		if (next->color == !current->color) {
			if (num_added > 0) {
				branch_points.push(next_entry);
			}
			num_added++;
			next_entry = entry + DOWN;
		}
	}
	if (current->left == 1) {
		next = p.get_node(ind_index, entry + LEFT);
		/*std::cout << "left ";
		std::cout << current->left << std::endl;
		std::cin.get();*/
		if (next->color == !current->color) {
			if (num_added > 0) {
				branch_points.push(next_entry);
			}
			num_added++;
			next_entry = entry + LEFT;
		}
	}
	if (current->right == 1) {
		next = p.get_node(ind_index, entry + RIGHT);
		/*std::cout << "right ";
		std::cout << current->right << std::endl;
		std::cin.get();*/
		if (next->color == !current->color) {
			if (num_added > 0) {
				branch_points.push(next_entry);
			}
			num_added++;
			next_entry = entry + RIGHT;
		}
	}

	if (num_added == 0) {
		if (branch_points.size() > 0) {
			next_entry = branch_points.top();
			branch_points.pop();
			next = p.get_node(ind_index, next_entry);
		}
		else {
			return pos(-1, -1);
		}
	}
	return next_entry;
}

void remove_color(Population &p, int ind_index, pos entry, std::stack<pos> &branch_points) {
	pos next = traverse_ST(p, ind_index, entry, branch_points);
	while (next.x != static_cast<unsigned short>(-1)) {
		next = traverse_ST(p, ind_index, next, branch_points);
	}
}
