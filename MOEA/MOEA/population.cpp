#include<iostream>
#include<queue>
#include"fitness.h"
#include"global.h"
#include"population.h"


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
		for (int row_i = 0; row_i < im_w; row_i++) {
			population[ind_i][row_i] = new node[im_h]{};
		}
	}
	
	
}

node** Population::get_individual(int ind_index) {
	return this->population[ind_index];
}

RGB Population::get_RGB(int x , int y) {
	int r = im.at<cv::Vec3b>(y, x)[2];
	int g = im.at<cv::Vec3b>(y, x)[1];
	int b = im.at<cv::Vec3b>(y, x)[0];
	return RGB(r,g, b);
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
		edge(direction e, int x, int y, double RGBdist) : e(e), origin(pos(x,y)), RGBdist(RGBdist) {};
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

		void add_uncolored_edges_to_heap(min_queue &minHeap, Population &p, int ind_index, int x, int y) {
			node **graph = p.get_individual(ind_index);
			node *n = &graph[x][y];
			int width = p.get_im_w();
			int height = p.get_im_h();

			n->color = 1;

			if (x + 1 < width) {
				std::cout << n->color << ", n; " << graph[x + 1][y].color << ", other";
				if (n->color != graph[x + 1][y].color) {
					std::cout << " chosen.";
					minHeap.emplace(RIGHT, x, y, dist(p.get_RGB(x, y), p.get_RGB(x + 1, y)));
				}
			}
			if (x > 0) {
				std::cout << std::endl << n->color << ", n; " << graph[x - 1][y].color << ", other";
				if (n->color != graph[x - 1][y].color) {
					std::cout << " chosen.";
					minHeap.emplace(LEFT, x, y, dist(p.get_RGB(x, y), p.get_RGB(x - 1, y)));
				}
			}
			if (y + 1 < height) {
				std::cout << std::endl << n->color << ", n; " << graph[x][y + 1].color << ", other";
				if (n->color != graph[x][y + 1].color) {
					std::cout << " chosen.";
					minHeap.emplace(UP, x, y, dist(p.get_RGB(x, y), p.get_RGB(x, y + 1)));
				}
			}
			if (y > 0) {
				std::cout << std::endl << n->color << ", n; " << graph[x][y - 1].color << ", other";
				if (n->color != graph[x][y - 1].color) {
					std::cout << " chosen.";
					minHeap.emplace(DOWN, x, y, dist(p.get_RGB(x, y), p.get_RGB(x, y - 1)));
				}
			}
			std::cout << std::endl;
			//std::cin.get();
		}
	};
	color_functor functor;

	for (int ind_index = 0; ind_index < N_IND; ind_index++) {
		{
			int x = rand() % this->get_im_w();
			int y = rand() % this->get_im_h();
			std::cout << x << ", x\n" << y << ", y." << std::endl;
			//std::cin.get();*/
			functor.add_uncolored_edges_to_heap(minHeap, *this, ind_index, x, y);
		}
		unsigned int num_colored = 1;
		while (num_colored < get_im_w()*get_im_h()) {
			/*while (!minHeap.empty()) {
				std::cout << minHeap.top().RGBdist << std::endl;
				minHeap.pop();
			}*/
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
			functor.add_uncolored_edges_to_heap(minHeap, *this, ind_index, next_pos.x, next_pos.y);
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