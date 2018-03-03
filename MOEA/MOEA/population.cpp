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

/*struct KeyDataHasher
{
	std::size_t operator () (const KeyData &key) const
	{
		// The following line is a stright forward implementation. But it can be
		// hard to design a good hash function if KeyData is complex.

		//return (key.id << 32 | key.age); // suppose size_t is 64-bit and int is 32-bit

		// A commonly used way is to use boost
		std::size_t seed = 0;
		boost::hash_combine(seed, boost::hash_value(key.id));
		boost::hash_combine(seed, boost::hash_value(key.age));
		return seed;
	}
};*/
/*
struct hash<edge>
{
	std::size_t operator()(const edge& k) const
	{
		using std::size_t;
		using std::hash;
		using std::string;

		return ((hash<int>()(k.p1.x)) ^ (hash<int>()(k.p1.y)) ^ (hash<int>()(k.p2.x)) ^ (hash<int>()(k.p2.y) )^ (hash<int>()(k.RGBdist)));
	}
};*/

struct edge {
	pos p1;
	pos p2;
	double RGBdist = 0;
	int dependent_nodes = 0;
	edge(pos p1, pos p2, double RGBdist) : p1(p1), p2(p2), RGBdist(RGBdist) {};
	edge(pos p1, pos p2, double RGBdist, int n_nodes) : p1(p1), p2(p2), RGBdist(RGBdist), dependent_nodes(n_nodes) {};
};

struct edge_comparator {
	bool operator()(edge e1, edge e2) {
		return e1.RGBdist < e2.RGBdist;
	}
};

bool operator<(edge e1, edge e2) {
	return e1.RGBdist < e2.RGBdist;
}

Population::~Population() {
	std::vector<pos> * edge_candidates();
	for (int ind_i = 0; ind_i < N_IND; ind_i++) {
		for (int col_i = 0; col_i < get_im_w(); col_i++) {
			delete [] population[ind_i][col_i] ;
		}
		delete[] population[ind_i];
	}
	delete[] population;

	//delete[] edge_candidates;
	delete[] entry_s;
}

Population::Population() {
	
	this->im = cv::imread(img_path, 1);   // Read the file
	//this->im = test_image();
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

	//JOINING SEGMENT 1 AND 2:
	population[0][1][1].right = true;
	population[0][2][1].left = true;


	//Segment 2
	population[0][2][0].down = true;
	population[0][2][1].up = true;

	population[0][2][1].down = true;
	population[0][2][2].up = true;
	set_num_children(*this, 0, { 0, 0 });
}


// Driver program to test methods of graph class
void Population::initialize_population_PrimsMST_2(){

	int ind_index = 0;

	time_t seconds = time(NULL);
	
	// create the graph given in above fugure
	int V = get_im_w()*get_im_h();
	Graph g(V);
	
	std::vector<int> check(get_im_w()*get_im_h(),0);
	for (int x = 0; x < get_im_w(); x++) {
		for (int y = 0; y < get_im_h(); y++) {
			if (x + 1 < get_im_w()) {
				g.addEdge(x*get_im_h() + y, (x + 1)*get_im_h() + y, dist(get_RGB(pos(x, y)), get_RGB(pos(x + 1, y))));
			}
			if (y + 1 < get_im_h()) {
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
	
	std::priority_queue<edge, std::vector<edge>, edge_comparator> que;
	for (int i = 1; i < get_im_h()*get_im_w(); i++) {
		int index = mst_parents[i];
		int x1 = index / get_im_h();
		int y1 = index - get_im_h()*x1;
		pos p1(x1, y1);

		index = i;
		int x2 = index / get_im_h();
		int y2 = index - get_im_h()*x2;
		pos p2(x2, y2);
		que.emplace(p1,p2, dist(get_RGB(p1), get_RGB(p2)));
		set_dir_edge(p1, p2, 1,ind_index);	
	}

	std::cout << "initialization  children" << std::endl;
	std::cout << seconds - time(NULL) << std::endl;
	seconds = time(NULL);

	set_num_children(*this, 0, { 0, 0 });
	//initialize_n_children(0, edgeChildren);

	int segment_size = 1000; // three = 1000
	int n_segments = 100; // how many segments currently made // three 100
	create_segments(ind_index, segment_size, que, n_segments);
	segment_size = 10000; // three 10000
	n_segments = 60; // how many segments currently made // three 60
	create_segments(ind_index, segment_size, que, n_segments);
	

	std::cout << "entry_s size " << entry_s[ind_index].size() << std::endl;
	int total_segment_size = 0;
	for (auto it = entry_s[0].begin(); it != entry_s[ind_index].end();) {
		//std::cout << " check " << it->x << " " << it->y << std::endl;
		int segment_size = set_segment_value(*it, ind_index);
		if (segment_size == 0) {
			it = entry_s[ind_index].erase(it);
			//std::cout << " zero in size" << std::endl;
			continue;
		}
		else {
			std::cout <<"segment " << it->x << " " << it->y << " " << segment_size << std::endl;
			it++;
		}
		total_segment_size += segment_size;

	}

	std::cout << "total " << total_segment_size << "should be " << get_im_h()*get_im_w() << std::endl;

	/*for (auto it = entry_s[0].begin(); it != entry_s[0].end();it++) {
	test_segment(*it, 0);
	}*/

	std::cout << "FINISHED " << std::endl;
	std::cout << seconds - time(NULL) << std::endl;
	seconds = time(NULL);
	draw_segments(ind_index);
	draw_segments_contour(ind_index);
	cv::waitKey(0);
}

void Population::set_dir_edge(pos& parent, pos& child, int on, int ind_index) {
	int x1 = parent.x;
	int x2 = child.x;
	int y1 = parent.y;
	int y2 = child.y;
	if (x1 > x2) {
		population[ind_index][x1][y1].left = on;
		population[ind_index][x2][y2].right = on;
		if(on == 1) population[ind_index][x2][y2].parent_dir = RIGHT ;
	}
	else if (x1 < x2) {
		population[ind_index][x1][y1].right = on;
		population[ind_index][x2][y2].left = on;
		if (on == 1) population[ind_index][x2][y2].parent_dir = LEFT;
	}
	else if (y1 > y2) {
		population[ind_index][x1][y1].up = on;
		population[ind_index][x2][y2].down = on;
		if (on == 1) population[ind_index][x2][y2].parent_dir = DOWN;
	}
	else {
		population[ind_index][x1][y1].down = on;
		population[ind_index][x2][y2].up = on;
		if (on == 1) population[ind_index][x2][y2].parent_dir = UP;
	}

	if( on == 0) population[ind_index][x2][y2].parent_dir = SELF;
}

int Population::set_segment_value(pos& entry, int ind_index) {
	stack<pos> branch_points;

	pos invalid_pos(-1, -1);
	if (population[ind_index][entry.x][entry.y].entry != invalid_pos) {
		//std::cout << "You taken by " << entry.x << " " << entry.y << std::endl;
		return 0;
	}
	else {
		//std::cout << "you not taken " << std::endl;
		population[ind_index][entry.x][entry.y].entry = entry;
		pos next = entry;
		int count = 0;
		while (next.x != static_cast<unsigned short>(-1)) {
			population[ind_index][next.x][next.y].entry = entry;
			next = traverse_ST(*this, ind_index, next, branch_points);
			count++;
		}
		if(count == 0) population[ind_index][entry.x][entry.y].entry = invalid_pos;
		remove_color(*this, ind_index, entry, branch_points);
		return count;
	}
}

int Population::get_n_segment(pos& entry, int ind_index) {
	stack<pos> branch_points;

	pos invalid_pos(-1, -1);
	if (population[ind_index][entry.x][entry.y].entry != invalid_pos) {
		//std::cout << " taken by " << entry.x << " " << entry.y << std::endl;
		return 0;
	}
	else {
		pos next = traverse_ST(*this, ind_index, entry, branch_points);
		int count = 1;
		while (next.x != static_cast<unsigned short>(-1)) {
			count++;
			next = traverse_ST(*this, ind_index, next, branch_points);
		}
		remove_color(*this, ind_index, entry, branch_points);
		return count;
	}
}

void Population::change_parents_n_segment(pos& parent, pos& child, int ind_index) {
	population[ind_index][parent.x][parent.y].num_children -= population[ind_index][child.x][child.y].num_children;
	pos next = pos(parent.x, parent.y) + population[ind_index][parent.x][parent.y].parent_dir;
	while (next.x != static_cast<unsigned short>(-1) && population[ind_index][next.x][next.y].parent_dir != SELF) {
		population[ind_index][next.x][next.y].num_children -= population[ind_index][child.x][child.y].num_children;
		next = pos(next.x, next.y) + population[ind_index][next.x][next.y].parent_dir;
	}
	population[ind_index][next.x][next.y].num_children -= population[ind_index][child.x][child.y].num_children; // rootnode
}

int Population::get_parent_segment_size(pos& parent, int ind_index) {
	pos next = pos(parent.x, parent.y) + population[ind_index][parent.x][parent.y].parent_dir;

	while (next.x != static_cast<unsigned short>(-1) && population[ind_index][next.x][next.y].parent_dir != SELF) {
		next = pos(next.x, next.y) + population[ind_index][next.x][next.y].parent_dir;
	}	
	return population[ind_index][next.x][next.y].num_children;
}

void Population::test_segment(pos& entry, int ind_index) {
	stack<pos> branch_points;

	pos invalid_pos(-1, -1);

	std::vector<int> temp(get_im_h()*get_im_w(), 0);
	pos next = traverse_ST(*this, ind_index, entry, branch_points);
	while (next.x != static_cast<unsigned short>(-1)) {
		temp[next.x*get_im_h() + next.y] += 1;
		if (temp[next.x*get_im_h() + next.y] > 1) std::cout << "visited more " << std::endl;
		next = traverse_ST(*this, ind_index, next, branch_points);
	}
	remove_color(*this, ind_index, entry, branch_points);
}

void Population::draw_segments(int ind_index) {
	cv::Mat segment(get_im_h(), get_im_w(), CV_8UC3, cv::Scalar(255, 255, 255));

	std::vector<RGB> color;
	color.push_back(RGB(0, 255, 0));
	color.push_back(RGB(255, 255, 0));
	color.push_back(RGB(255, 0, 0));
	color.push_back(RGB(255, 0, 255));
	color.push_back(RGB(255, 255, 255));
	color.push_back(RGB(0, 255, 255));
	color.push_back(RGB(0, 0, 255));
	color.push_back(RGB(120, 120, 120));
	color.push_back(RGB(0, 120, 120)); // turquois
	color.push_back(RGB(0, 0, 120));
	std::cout << " test imshow " << std::endl;
	int count = 0;
	for (auto it = entry_s[ind_index].begin(); it != entry_s[ind_index].end(); it++) {
		stack<pos> branch_points;
		pos next = *it;
		int count_pixels = 0;
		while (next.x != static_cast<unsigned short>(-1)) {
			segment.at<cv::Vec3b>(next.y, next.x)[2] = color[count].r;
			segment.at<cv::Vec3b>(next.y, next.x)[1] = color[count].g;
			segment.at<cv::Vec3b>(next.y, next.x)[0] = color[count].b;
			next = traverse_ST(*this, ind_index, next, branch_points);
			count_pixels++;
		}
		std::cout << count << " " <<it->x << " "<< it->y << " " << count_pixels << std::endl;
		count++;
		if (count == 10) count = 0;
	}
	std::cout << "show segment" << std::endl;
	cv::namedWindow("image", 1);
	//std::cout << "show image" << std::endl;
	cv::imshow("image", segment);                   
	//cv::waitKey(0);
}

void Population::draw_segments_contour(int ind_index) {
	cv::Mat segment = cv::imread(img_path, 1);

	std::vector<pos>* edge_segment = edges_segment(ind_index);
	RGB color(0, 255, 0); // green ?=
	
	std::cout << " test imshow " << std::endl;
	for (int i = 0; i < entry_s[ind_index].size(); i++) {
		for (auto it = edge_segment[i].begin(); it != edge_segment[i].end(); ++it) {
			//std::cout << " it->y" << it->y << " " << it->x << std::endl;
			segment.at<cv::Vec3b>(it->y, it->x)[2] = color.r;
			segment.at<cv::Vec3b>(it->y, it->x)[1] = color.g;
			segment.at<cv::Vec3b>(it->y, it->x)[0] = color.b;
		}
		//std::cout << "i " << i << std::endl;
	}
	delete[] edge_segment;
	//std::cout << "show image" << std::endl;
	cv::namedWindow("contour", 1);
	//std::cout << "show image" << std::endl;
	cv::imshow("contour", segment);
}

cv::Mat test_image() {
	int size = 4;
	cv::Mat im(size, size, CV_8UC3, cv::Scalar(255, 255, 255));

	for (int x = 0; x < size/2.0; x++) {
		for (int y = 0; y < size; y++) {
			im.at<cv::Vec3b>(y, x)[2] = 0;
			im.at<cv::Vec3b>(y, x)[1] = 0;
			im.at<cv::Vec3b>(y, x)[0] = 0;
		}
		
	}

	/*for (int x = 0; x < 45; x++) {
		for (int y = 0; y < 45; y++) {
			im.at<cv::Vec3b>(y, x)[2] = 0;
			im.at<cv::Vec3b>(y, x)[1] = 0;
			im.at<cv::Vec3b>(y, x)[0] = 0;
		}
	}

	for (int x = 45; x < 90; x++) {
		for (int y = 45; y < 90; y++) {
			im.at<cv::Vec3b>(y, x)[2] = 0;
			im.at<cv::Vec3b>(y, x)[1] = 0;
			im.at<cv::Vec3b>(y, x)[0] = 0;
		}
	}*/



	/*im.at<cv::Vec3b>(0, 0)[2] = 0;
	im.at<cv::Vec3b>(0, 0)[1] = 0;
	im.at<cv::Vec3b>(0, 0)[0] = 0;

	im.at<cv::Vec3b>(1, 0)[2] = 0;
	im.at<cv::Vec3b>(1, 0)[1] = 0;
	im.at<cv::Vec3b>(1, 0)[0] = 0;

	im.at<cv::Vec3b>(2, 0)[2] = 0;
	im.at<cv::Vec3b>(2, 0)[1] = 0;
	im.at<cv::Vec3b>(2, 0)[0] = 0;*/

	cv::namedWindow("image", 1);
	//std::cout << "show image" << std::endl;
	cv::imshow("image", im);
	cv::waitKey(0);
	return im;
}


std::vector<pos>* Population::edges_segment(int ind_index) {
	std::vector<pos>* segment = new std::vector<pos>[entry_s->size()];
	stack<pos> branch_points;

	int count = 0;
	for (auto it = entry_s[ind_index].begin(); it != entry_s[ind_index].end(); ++it) {
		pos next = *it;
		int count_pixels = 0;
		int cout;
		if (next.x == 35 && next.y == 317) {
			std::cout << " in segment " << std::endl;
			cout = 1;
		}
		else {
			cout = 0;
		}
		while (next.x != static_cast<unsigned short>(-1)) {
			if (check_if_edge(next,ind_index,cout) == 1) {
				segment[count].push_back(next);
				//std::cout << " new edge" << std::endl;
			}
			next = traverse_ST(*this, ind_index, next, branch_points);
			count_pixels++;
		}
		std::cout << it->x << " " <<it->y << " "<< count_pixels << " edges " << segment[count].size() <<std::endl;
		count++;
		remove_color(*this, ind_index, *it, branch_points);
	}	
	return segment;
}

void Population::create_segments(int ind_index, int segment_size, edge_priority_que& que, int n_segments) {

	while (n_segments > 0 && !que.empty()) {
		edge temp = que.top();
		que.pop();
		pos parent;
		pos child;
		pos parent_for_p1 = (temp.p1 + population[ind_index][temp.p1.x][temp.p1.y].parent_dir);
		if (parent_for_p1 == temp.p2) {
			parent = temp.p2;
			child = temp.p1;
		}
		else {
			parent = temp.p1;
			child = temp.p2;
		}
		int n_segment_2 = get_parent_segment_size(parent,ind_index);
		int n_segment = population[ind_index][child.x][child.y].num_children;

		

		if (n_segment_2 - n_segment> segment_size && n_segment> segment_size) {
			
			//std::cout << "parent" << parent.x << " " << parent.x << " child " << child.x << " " << child.y << std::endl;
			set_dir_edge(parent, child, 0,ind_index);
			//std::cout << "n_pixels " << population[ind_index][child.x][child.y].num_children;
			//std::cout << " get_ parent " << get_n_segment(parent, ind_index) << " calculate " << n_segment_2 - n_segment << std::endl;
			//std::cout << " get_ child " << get_n_segment(child, ind_index) << " calculate " <<  n_segment << std::endl;
			change_parents_n_segment(parent, child, ind_index);
			//int min_pixels = get_n_segment(child, ind_index);
			//std::cout << " get_n " << min_pixels << std::endl;
			entry_s[ind_index].push_back(child);
			entry_s[ind_index].push_back(parent);
			n_segments--;
		}
	}
}

int Population::check_if_edge(pos curr,  int ind_index, int cout) {

	if (curr.x == 0 || curr.y == 0 || curr.x + 1 == get_im_w() || curr.y +1 == get_im_h()) {
		return 1;
	}

	if (population[ind_index][curr.x][curr.y].left == 0 && curr.x- 1 > 0) {
		pos neighbour = curr + LEFT;
		//pos temp = population[ind_index][neighbour.x][neighbour.y].entry;
		//if (cout) std::cout << population[ind_index][curr.x][curr.y].entry.x <<" "<< population[ind_index][curr.x][curr.y].entry.x <<" " << temp.x << " " <<temp.y << std::endl;
 		if (population[ind_index][neighbour.x][neighbour.y].entry != population[ind_index][curr.x][curr.y].entry) {
		//	if (cout) std::cout << " new edge" << std::endl;
			return 1;
		}
	}

	if (population[ind_index][curr.x][curr.y].right == 0 && curr.x + 1 < get_im_w()) {
		pos neighbour = curr + RIGHT;
		//pos temp = population[ind_index][neighbour.x][neighbour.y].entry;
		//if (cout) std::cout << population[ind_index][curr.x][curr.y].entry.x << " " << population[ind_index][curr.x][curr.y].entry.x << " " << temp.x << " " << temp.y << std::endl;
		if (population[ind_index][neighbour.x][neighbour.y].entry != population[ind_index][curr.x][curr.y].entry) {
		//	if (cout) std::cout << " new edge" << std::endl;
			return 1;
		}
	}

	if (population[ind_index][curr.x][curr.y].up == 0 && curr.y - 1 > 0) {
		pos neighbour = curr + UP;
		//pos temp = population[ind_index][neighbour.x][neighbour.y].entry;
		//if (cout) std::cout << population[ind_index][curr.x][curr.y].entry.x << " " << population[ind_index][curr.x][curr.y].entry.x << " " << temp.x << " " << temp.y << std::endl;
		if (population[ind_index][neighbour.x][neighbour.y].entry != population[ind_index][curr.x][curr.y].entry) {
		//	if (cout) std::cout << " new edge" << std::endl;
			return 1;
		}
	}

	if (population[ind_index][curr.x][curr.y].down == 0 && curr.y + 1 < get_im_h()) {
		pos neighbour = curr + DOWN;
		//pos temp = population[ind_index][neighbour.x][neighbour.y].entry;
		//if (cout) std::cout << population[ind_index][curr.x][curr.y].entry.x << " " << population[ind_index][curr.x][curr.y].entry.x << " " << temp.x << " " << temp.y << std::endl;
		if (population[ind_index][neighbour.x][neighbour.y].entry != population[ind_index][curr.x][curr.y].entry) {
			//if (cout) std::cout << " new edge" << std::endl;
			return 1;
		}
	}
	
	return 0;
	
}