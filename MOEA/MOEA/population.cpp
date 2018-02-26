#include<iostream>
#include<queue>
#include"fitness.h"
#include"global.h"
#include"population.h"
#include <list>
#include <time.h>
#include "graph.h"

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
	entry_s = new std::vector<pos>[N_IND];
	
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


void Population::initialize_population_test() {
	population = new node**[1];
	population[0] = new node*[3];
	for (int i = 0; i < 3; i++) {
		population[0][i] = new node[3];
	}

	//Segment 1
	population[0][0][0].right = true;
	population[0][1][0].left = true;

	population[0][1][0].down = true;
	population[0][1][1].up = true;

	population[0][1][1].left = true;
	population[0][0][1].right = true;

	population[0][1][1].down = true;
	population[0][1][2].up = true;

	population[0][1][2].left = true;
	population[0][0][2].right = true;

	//Segment 2
	population[0][2][0].down = true;
	population[0][2][1].up = true;

	population[0][2][1].down = true;
	population[0][2][2].up = true;
}



/*void Population::read_image(Mat* img) {

	
	//cv::namedWindow("image", 1);
	//cv::imshow("image", image);                   // Show our image inside it.	
	//cv::waitKey(0);
}*/


// Driver program to test methods of graph class
void Population::initialize_population_PrimsMST_2(){

	struct edge {
		pos p1;
		pos p2;
		double RGBdist = 0;
		edge(pos p1, pos p2, double RGBdist) : p1(p1), p2(p2), RGBdist(RGBdist) {};
	};

	struct edge_comparator {
		bool operator()(edge e1, edge e2) {
			return e1.RGBdist < e2.RGBdist;
		}
	};

	time_t seconds = time(NULL);
	
	// create the graph given in above fugure
	int V = get_im_w()*get_im_h();
	Graph g(V);
	

	std::vector<int> check(get_im_w()*get_im_h(),0);
	for (int x = 0; x < get_im_w(); x++) {
		for (int y = 0; y < get_im_h(); y++) {
			if (x + 1 < get_im_w()) {
				//std::cout << im.cols << " " << x <<" "  << x+1 <<std::endl;
				//std::cout << im.rows << " " << y << std::endl;
				g.addEdge(x*get_im_h() + y, (x + 1)*get_im_h() + y, dist(get_RGB(pos(x, y)), get_RGB(pos(x + 1, y))));
			}
			if (y + 1 < get_im_h()) {
				//std::cout << im.cols << " " << x << std::endl;
				//std::cout << im.rows << " " << y << " " <<y+1 <<std::endl;
				g.addEdge(x*get_im_h() + y + 1, (x)*get_im_h() + y, dist(get_RGB(pos(x, y)), get_RGB(pos(x, y + 1))));
			}
		}
	}
	std::cout << "initilization " << std::endl;
	std::cout << seconds- time(NULL) << std::endl;
	seconds = time(NULL);
	
	//  making above shown graph

	std::vector<int> mst_parents = g.primMST(); // holdes parents of each edge

	//std::cout << mst_parents.size() << " vs. size " << get_im_h()*get_im_w() << std::endl;

	std::cout << "MST " << std::endl;
	std::cout << seconds - time(NULL) << std::endl;
	seconds = time(NULL);

	/*for (int i = 1; i < get_im_h()*get_im_w(); ++i){
		check[i] += 1;
		if (mst_parents[i] == -1) {
			std::cout << " something wrong " << std::endl;
		}
		check[mst_parents[i]] += 1;
		if (check[i] > 4 || check[mst_parents[i]] > 4) {
			std::cout << "vertex wrong" << std::endl;
			printf("%d - %d\n", check[mst_parents[i]], check[i]);
		}
	}

	for (int i = 1; i < get_im_h()*get_im_w(); ++i) {
		if (check[i] == 0 ) {
			std::cout << "vertex not in " << i <<std::endl;
		}
	}*/
	
	std::priority_queue<edge, std::vector<edge>, edge_comparator> que;
	for (int i = 1; i < get_im_h()*get_im_w(); i++) {
		int index = mst_parents[i];
		int x1 = index / get_im_h();
		int y1 = index - get_im_h()*x1;
		pos p1(x1, y1);
		//std::cout << " index " << index << std::endl;

		index = i;
		int x2 = index / get_im_h();
		int y2 = index - get_im_h()*x2;
		pos p2(x2, y2);
		//std::cout << " index " << index << std::endl;
		//std::cout << x1 << " " << x2 << " " << get_im_w() << std::endl;
		//std::cout << y1 << " " << y2 << " " << get_im_h() << std::endl;
		que.emplace(p1,p2, dist(get_RGB(p1), get_RGB(p2)));
		if (x1 > x2) {
			population[0][x1][y1].left= 1;
			population[0][x2][y2].right = 1;
		}else if (x1 < x2) {
			population[0][x1][y1].right = 1;
			population[0][x2][y2].left = 1;
		}else if (y1 > y2) {
			population[0][x1][y1].up = 1;
			population[0][x2][y2].down = 1;
		}
		else {
			population[0][x1][y1].down = 1;
			population[0][x2][y2].up = 1;
		}	
	}
	for (int i = 0; i < N_SEG; i++) {
		edge temp = que.top();
		que.pop();
		int x1 = temp.p1.x;
		int x2 = temp.p2.x;
		int y1 = temp.p1.y;
		int y2 = temp.p2.y;
		entry_s[0].push_back(pos(x1, y1));
		entry_s[0].push_back(pos(x2, y2));
		if (x1 > x2) {
			population[0][x1][y1].left = 0;
			population[0][x2][y2].right = 0;
		}
		else if (x1 < x2) {
			population[0][x1][y1].right = 0;
			population[0][x2][y2].left = 0;
		}
		else if (y1 > y2) {
			population[0][x1][y1].up = 0;
			population[0][x2][y2].down = 0;
		}
		else {
			population[0][x1][y1].down = 0;
			population[0][x2][y2].up = 0;
		}
	}
	
	std::cout << "FINISHED " << std::endl;
	std::cout << seconds - time(NULL) << std::endl;
	seconds = time(NULL);

}