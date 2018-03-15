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

// inspired by http://geeksquiz.com/implement-min-heap-using-stl/

using namespace std;

Graph::Graph(int V){
	this->V = V;
	adj = new list<diPair>[V];
}

Graph::~Graph(){
	delete[] adj;
}

void Graph::addEdge(int u, int v, double w){
	adj[u].push_back(make_pair(w, v));
	adj[v].push_back(make_pair(w, u));
}


vector<int> Graph::primMST(int start_index){
	priority_queue< diPair, vector <diPair>, greater<diPair> > pq;

	int src = start_index; 

	vector<double> key(V, INF);

	vector<int> parent(V, -1);

	vector<bool> inMST(V, false);

	pq.push(make_pair(0.0, src));
	key[src] = 0.0;

	while (!pq.empty())
	{

		int u = pq.top().second;
		pq.pop();

		inMST[u] = true;  

		for (auto i = adj[u].begin(); i != adj[u].end(); ++i)
		{

			int v = (*i).second;
			double weight = (*i).first;


			if (inMST[v] == false && key[v] > weight)
			{
				key[v] = weight;
				pq.push(make_pair(key[v], v));
				parent[v] = u;
			}
		}
	}

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


void set_num_children(Population &p, int ind_index, pos entry) {
	std::stack<pos> branch_points;
	pos prev(entry.x, entry.y);
	pos next(entry.x, entry.y);
	pos r_next;
	//p.get_node(ind_index, entry)->entry = entry;

	int counted_children;
	int last_num_branch_points = 0;
	bool backtrack_and_count_children = false;
	int num_visited = 0;
	while (prev.x != static_cast<unsigned short>(-1)) {
		if (!backtrack_and_count_children) {
			// Traverse and set parent
			next = traverse_ST(p, ind_index, next, branch_points);
			num_visited++;

			// Handle node found with no other children than itself
			if (last_num_branch_points > branch_points.size() || next.x == static_cast<unsigned short>(-1)) {
				backtrack_and_count_children = true;
				r_next = prev;
				counted_children = 1;
				continue;
			}
			//node *n_next = p.get_node(ind_index, next);

			// Set parent
			//if (prev.x < next.x) n_next->parent_dir = LEFT;
			//if (prev.x > next.x) n_next->parent_dir = RIGHT;
			//if (prev.y < next.y) n_next->parent_dir = UP;
			//if (prev.y > next.y) n_next->parent_dir = DOWN;

			prev = next;
			last_num_branch_points = branch_points.size();
		}
		else {
			// Set num_children
			node *n_r_next = p.get_node(ind_index, r_next);
			if (n_r_next->num_children > 0) n_r_next->num_children -= 1; // Don't count self twice
			n_r_next->num_children += counted_children;
			counted_children = n_r_next->num_children + 1;


			// Check for untraversed branch
			if (n_r_next->up)
				if(p.get_node(ind_index, r_next + UP)->color != n_r_next->color) backtrack_and_count_children = false;
			if (n_r_next->down)
				if(p.get_node(ind_index, r_next + DOWN)->color != n_r_next->color) backtrack_and_count_children = false;
			if (n_r_next->right)
				if(p.get_node(ind_index, r_next + RIGHT)->color != n_r_next->color) backtrack_and_count_children = false;
			if (n_r_next->left)
				if(p.get_node(ind_index, r_next + LEFT)->color != n_r_next->color) backtrack_and_count_children = false;
			if (backtrack_and_count_children && n_r_next->parent_dir == SELF) break;

			if(backtrack_and_count_children) { // Iterate backtracker 
				pos parent = r_next + n_r_next->parent_dir;
				r_next = parent;
			}
			else { // Else exit backtracking

				// Continue where we left off
				prev = r_next;

				// Set parent
				//node *n_next = p.get_node(ind_index, next);
				/*if (prev.x < next.x) n_next->parent_dir = LEFT;
				if (prev.x > next.x) n_next->parent_dir = RIGHT;
				if (prev.y < next.y) n_next->parent_dir = UP;
				if (prev.y > next.y) n_next->parent_dir = DOWN;*/

				prev = next;
				last_num_branch_points = branch_points.size();

			}
		}
	}
}

