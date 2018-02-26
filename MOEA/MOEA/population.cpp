#include<iostream>
#include<queue>
#include"fitness.h"
#include"global.h"
#include"population.h"
#include <list>

// opencv 
#include <opencv2/highgui/highgui.hpp>


Population::~Population() {

}

Population::Population() {

	this->im = cv::imread(img_path, 1);   // Read the file

	if (!im.data)                              // Check for invalid input
	{
		std::cout << "Could not open or find the image" << std::endl;
		exit(0);
	}

	int im_h = im.rows;
	int im_w = im.cols;
	population = new node**[N_IND];
	for (int ind_i = 0; ind_i < N_IND; ind_i++) {
		population[ind_i] = new node*[im_w];
		for (int col_i = 0; col_i < im_w; col_i++) {
			population[ind_i][col_i] = new node[im_h]{};
		}
	}
	
	
}

/*node** Population::get_individual(int ind_index) {
	return this->population[ind_index];
}*/

node* Population::get_node(int ind_index, pos node_pos) {
	return &this->population[ind_index][node_pos.x][node_pos.y];
}

RGB Population::get_RGB(int x , int y) {
	int r = im.at<cv::Vec3b>(y, x)[2];
	int g = im.at<cv::Vec3b>(y, x)[1];
	int b = im.at<cv::Vec3b>(y, x)[0];
	return RGB(r,g, b);
}
RGB Population::get_RGB(pos pixel_pos) {
	return get_RGB(pixel_pos.x, pixel_pos.y);
}

int Population::get_im_w() {
	return this->im.cols;
}
int Population::get_im_h() {
	return this->im.rows;
}

pos Population::get_pixel_segment(int x,int y, int individual) {
	return this->population[individual][x][y].entry;
}


void Population::initialize_population_PrimsMST() {

	struct edge {

		direction e = SELF;
		pos origin;
		double RGBdist = 0;
		edge(direction e, pos origin, double RGBdist) : e(e), origin(origin), RGBdist(RGBdist) {};
	};

	struct edge_comparator {
		bool operator()(edge e1, edge e2) {
			return e1.RGBdist > e2.RGBdist;
		}
	};

	typedef std::priority_queue<edge, std::vector<edge>, edge_comparator> min_queue;
	min_queue minHeap;

	/*// Test code
	node **graph;
	graph = new node*[3];
	for (int i = 0; i < 3; i++) {
	graph[i] = new node[3];
	}
	int height = 3;
	int width = 3;*/

	/*minHeap.emplace(edge_information{ UP, 2, 2, 10 });
	minHeap.emplace(edge_information{ UP, 2, 2, -2.2 });
	minHeap.emplace(edge_information{ DOWN, 3, 2, 3 });
	std::cout << minHeap.top().RGBdist << std::endl;
	std::cout << "Her!" << std::endl;
	std::cin.get();*/

	struct color_functor {

		void add_uncolored_edges_to_heap(min_queue &minHeap, Population &p, int ind_index, pos node_pos) {
			node *n = p.get_node(ind_index, node_pos);
			unsigned short width = p.get_im_w();
			unsigned short height = p.get_im_h();

			n->color = 1;
			std::cout << "SIZE " << minHeap.size() << std::endl;
			if (node_pos.x + 1 < width) {
				std::cout << n->color << ", n; " << p.get_node(ind_index, node_pos + RIGHT)->color << ", other";
				if (n->color != p.get_node(ind_index, node_pos + RIGHT)->color) {
					std::cout << " chosen.";
					minHeap.emplace(RIGHT, node_pos, dist(p.get_RGB(node_pos), p.get_RGB(node_pos + RIGHT)));
				}
			}
			else {
				std::cout << "Did not add RIGHT" << std::endl;
			}
			if (node_pos.x > 0) {
				std::cout << std::endl << n->color << ", n; " << p.get_node(ind_index, node_pos + LEFT)->color << ", other";
				if (n->color != p.get_node(ind_index, node_pos + LEFT)->color) {
					std::cout << " chosen.";
					minHeap.emplace(LEFT, node_pos, dist(p.get_RGB(node_pos), p.get_RGB(node_pos + LEFT)));
				}
			}
			else {
				std::cout << "Did not add LEFT" << std::endl;
			}
			if (node_pos.y + 1 < height) {
				std::cout << std::endl << n->color << ", n; " << p.get_node(ind_index, node_pos + DOWN)->color << ", other";
				if (n->color != p.get_node(ind_index, node_pos + DOWN)->color) {
					std::cout << " chosen.";
					minHeap.emplace(DOWN, node_pos, dist(p.get_RGB(node_pos), p.get_RGB(node_pos + DOWN)));
				}
			}
			else {
				std::cout << "Did not add DOWN" << std::endl;
			}
			if (node_pos.y > 0) {
				std::cout << std::endl << n->color << ", n; " << p.get_node(ind_index, node_pos + UP)->color << ", other";
				if (n->color != p.get_node(ind_index, node_pos + UP)->color) {
					std::cout << " chosen.";
					minHeap.emplace(UP, node_pos, dist(p.get_RGB(node_pos), p.get_RGB(node_pos + UP)));
				}
			}
			else {
				std::cout << "Did not add UP" << std::endl;
			}
			std::cout << std::endl;
			std::cout << "SIZE " << minHeap.size() << std::endl;
			//std::cin.get();
		}
	};
	color_functor functor;

	for (int ind_index = 0; ind_index < N_IND; ind_index++) {
		{
			unsigned short x = rand() % this->get_im_w();
			unsigned short y = rand() % this->get_im_h();
			pos entry(x, y);
			std::cout << x << ", x\n" << y << ", y." << std::endl;
			//std::cin.get();*/
			functor.add_uncolored_edges_to_heap(minHeap, *this, ind_index, entry);
		}
		unsigned int num_colored = 1;
		while (num_colored < get_im_w()*get_im_h()) {
			/*while (!minHeap.empty()) {
			std::cout << minHeap.top().RGBdist << std::endl;
			minHeap.pop();
			}*/
			std::cout << minHeap.size() << std::endl;
			direction add_edge = minHeap.top().e;
			pos at_pos = minHeap.top().origin;
			pos next_pos = at_pos + add_edge;
			minHeap.pop();
			std::cout << at_pos.x << " " << at_pos.y << std::endl;
			std::cout << next_pos.x << " " << next_pos.y << std::endl;
			std::cout << population[ind_index][at_pos.x][at_pos.y].color << " " << population[ind_index][next_pos.x][next_pos.y].color << std::endl;
			//std::cin.get();
			if (population[ind_index][next_pos.x][next_pos.y].color == population[ind_index][at_pos.x][at_pos.y].color) continue; // best edge has both vertices already added to the MST
			population[ind_index][at_pos.x][at_pos.y] + add_edge;
			functor.add_uncolored_edges_to_heap(minHeap, *this, ind_index, next_pos);
			// When unadded node points at added node, it means this edge is still in minHeap -> this slows down execution time because minHeap must sort all elements
			// But insert is O(1) on average, O(log n) worst case... So does it even matter?
			// It's defenitely taking way too long - so that's nothing to worry about. Something needs to improve
			// An alternative would be to make a segment when an MST gets big enough. But some segments may be optimally massive.
			// This may not be a problem however - because if two segments often get merged in mutations then it's fine
			// It's still not ideal though. Best case that i see right now would be to make one good one the correct way
			// then copy it into all other individuals - but it shouldn't take more time than like 2-10 minutes!
			// Though having run it - i can clearly see it struggling with removing tons and tons of edges that
			// points to a node already added to the MST. Pruning would hopefully help - though alternatively i could ask
			// someone who would know how to solve this...

			// I discovered an obvious bug - one edge kept being tried to be removed over and over again in an infinite loop.
			// So defenitely still something wrong (i have no idea how that's possible given minHeap.pop() should remove it,
			// and it can't be re-added (or shouldn't be a ble to))
			// However:
			// Removing edges as they no longer become necessary will definitely speed up execution time by a ton.
			// This is because even pop has logarithmic number of comparisons - so if the amount of edges become large,
			// then every pop will take tons of time, meaning it's much better to remove an illegal edge while the minHeap
			// is small, rather than when the minHeap is large!
			num_colored++;
		}
		std::cout << "Her!" << std::endl;
		std::cin.get();
	}

	std::cout << "Her!" << std::endl;
	std::cin.get();
}




/*void Population::read_image(Mat* img) {

	
	//cv::namedWindow("image", 1);
	//cv::imshow("image", image);                   // Show our image inside it.	
	//cv::waitKey(0);
}*/

using namespace std;

# define INF 0x3f3f3f3f

// iPair ==>  Integer Pair
typedef pair<int, int> iPair;

// This class represents a directed graph using
// adjacency list representation
class Graph
{
	int V;    // No. of vertices

			  // In a weighted graph, we need to store vertex
			  // and weight pair for every edge
	list< pair<int, int> > *adj;

public:
	Graph(int V);  // Constructor

				   // function to add an edge to graph
	void addEdge(int u, int v, int w);

	// Print MST using Prim's algorithm
	void primMST();
};

// Allocates memory for adjacency list
Graph::Graph(int V)
{
	this->V = V;
	adj = new list<iPair>[V];
}

void Graph::addEdge(int u, int v, int w)
{
	adj[u].push_back(make_pair(v, w));
	adj[v].push_back(make_pair(u, w));
}

// Prints shortest paths from src to all other vertices
void Graph::primMST()
{
	// Create a priority queue to store vertices that
	// are being preinMST. This is weird syntax in C++.
	// Refer below link for details of this syntax
	// http://geeksquiz.com/implement-min-heap-using-stl/
	priority_queue< iPair, vector <iPair>, greater<iPair> > pq;

	int src = 0; // Taking vertex 0 as source

				 // Create a vector for keys and initialize all
				 // keys as infinite (INF)
	vector<int> key(V, INF);

	// To store parent array which in turn store MST
	vector<int> parent(V, -1);

	// To keep track of vertices included in MST
	vector<bool> inMST(V, false);

	// Insert source itself in priority queue and initialize
	// its key as 0.
	pq.push(make_pair(0, src));
	key[src] = 0;

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
		list< pair<int, int> >::iterator i;
		for (i = adj[u].begin(); i != adj[u].end(); ++i)
		{
			// Get vertex label and weight of current adjacent
			// of u.
			int v = (*i).first;
			int weight = (*i).second;

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
	for (int i = 1; i < V; ++i)
		printf("%d - %d\n", parent[i], i);
}

// Driver program to test methods of graph class
void Population::initialize_population_PrimsMST_2()
{
	// create the graph given in above fugure
	int V = get_im_w()*get_im_h();
	Graph g(V);

	;
	for (int x = 0; x < get_im_w(); x++) {
		for (int y = 0; y < get_im_h(); y++) {
			if (x + 1 < get_im_w()) {
				//std::cout << im.cols << " " << x <<" "  << x+1 <<std::endl;
				//std::cout << im.rows << " " << y << std::endl;
				g.addEdge(x*get_im_h() + y, (x+1)*get_im_h() + y, dist(get_RGB(pos(x, y)), get_RGB(pos(x+1, y))));
			}
			if (y + 1 < get_im_h()) {
				//std::cout << im.cols << " " << x << std::endl;
				//std::cout << im.rows << " " << y << " " <<y+1 <<std::endl;
				g.addEdge(x*get_im_h() + y+1, (x)*get_im_h() + y, dist(get_RGB(pos(x, y)), get_RGB(pos(x, y+1))));
			}
		}
	}
	std::cout << "initilization " << std::endl;



	//  making above shown graph
	
	g.primMST();

}