#include"var_operators.h"
#include "var_operators.h"
#include"global.h"
#include "graph.h"
#include<iostream>
//#include<queue>
#include"fitness.h"
#include"population.h"
#include <list>
#include <time.h>
#include "selection.h"
#include "file.h"
//#include <opencv2/highgui/highgui.hpp>

// opencv 
#include <opencv2/highgui/highgui.hpp>


Population::~Population() {
	/*for (int ind_i = 0; ind_i < N_IND+N_OFFSPRING; ind_i++) {
		for (int col_i = 0; col_i < get_im_w(); col_i++) {
			delete [] population[ind_i][col_i] ;
		}
		delete[] population[ind_i];
	}
	delete[] population;

	//delete[] edge_candidates;
	delete[] entry_s;
	delete[] edge_candidates;*/
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
	population = new node**[N_IND+N_OFFSPRING];
	for (int ind_i = 0; ind_i < N_IND+ N_OFFSPRING; ind_i++) {
		population[ind_i] = new node*[im_w];
		for (int col_i = 0; col_i < im_w; col_i++) {
			population[ind_i][col_i] = new node[im_h]{};
		}
	}
	entry_s = new std::vector<pos>[N_IND+N_OFFSPRING];
	edge_candidates = new std::vector<active_edge_t>[N_IND+N_OFFSPRING];
	segment_prop = new std::map<pos,seg_prop_t>[N_IND + N_OFFSPRING];
	fitness_1.reserve(N_IND + N_OFFSPRING);
	fitness_2.reserve(N_IND + N_OFFSPRING);
	rank.reserve(N_IND + N_OFFSPRING);
}

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

void Population::test_prim() {
	int V = 9;
	Graph g(V);
	int start_mst_x = rand() % 8;
	std::cout << " start " << start_mst_x << std::endl;
	//  making above shown graph
	g.addEdge(0, 1, 4);
	g.addEdge(0, 7, 8);
	g.addEdge(1, 2, 8);
	g.addEdge(1, 7, 11);
	g.addEdge(2, 3, 7);
	g.addEdge(2, 8, 2);
	g.addEdge(2, 5, 4);
	g.addEdge(3, 4, 9);
	g.addEdge(3, 5, 14);
	g.addEdge(4, 5, 10);
	g.addEdge(5, 6, 2);
	g.addEdge(6, 7, 1);
	g.addEdge(6, 8, 6);
	g.addEdge(7, 8, 7);

	std::vector<int> parent = g.primMST(start_mst_x);
	for (int i = 0; i < V; ++i)
		printf("%d - %d\n", parent[i], i);
}

// Driver program to test methods of graph class
void Population::initialize_individual_PrimsMST(int ind_index){

	//time_t seconds = time(NULL);
	
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
				g.addEdge(x*get_im_h() + (y + 1), (x)*get_im_h() + y, dist(get_RGB(pos(x, y)), get_RGB(pos(x, y + 1))));
			}
		}
	}
	//std::cout << "initilization " << std::endl;
	//std::cout << seconds- time(NULL) << std::endl;
	//seconds = time(NULL);
	
	int start_mst_x = rand() % get_im_w();
	int start_mst_y = rand() % get_im_h();
	int start_index = start_mst_x * get_im_h() + start_mst_y;
	std::cout << " start index " << start_index << std::endl;

	
	std::vector<int> mst_parents = g.primMST(start_index); // MAKE PRIM MST

	//std::cout << "MST " << std::endl;
	//std::cout << seconds - time(NULL) << std::endl;
	//seconds = time(NULL);
	
	// SETTING EDGES IN PHENOTYPE
	std::priority_queue<edge, std::vector<edge>, edge_comparator> que;
	for (int i = 0; i < get_im_h()*get_im_w(); i++) {
		int index = i;
		int x2 = index / get_im_h();
		int y2 = index - get_im_h()*x2;
		pos p2(x2, y2);

		index = mst_parents[i];
		if (index == -1) {
			//std::cout << "parent " << index << "for " << i << std::endl;// one node will not have parents
			continue;
		}
		int x1 = index / get_im_h();
		int y1 = index - get_im_h()*x1;
		pos p1(x1, y1);

		//if (x1 >= get_im_w() || x2 >= get_im_w() || y1 >= get_im_h() || y2 >= get_im_h()) std::cout << " x1 " << x1 << " y1 " << y1 << " x2" << x2 << " y2 " << y2 << std::endl;
		que.emplace(p1,p2, dist(get_RGB(p1), get_RGB(p2)));
		set_dir_edge_and_parent(p1, p2, 1, ind_index);	
	}

	


	//std::cout << "initialization  children" << std::endl;
	//std::cout << seconds - time(NULL) << std::endl;
	//seconds = time(NULL);

	// setting number of children in graph below a parent
	set_num_children(*this, ind_index, pos(start_mst_x, start_mst_y));
	int i = 10;
	std::cout << " start node num children " << population[ind_index][start_mst_x][start_mst_y].num_children << std::endl;
	if(population[ind_index][start_mst_x][start_mst_y].left ) std::cout << " random left " << population[ind_index][start_mst_x - 1][start_mst_y ].num_children << std::endl;
	if (population[ind_index][start_mst_x][start_mst_y].right)std::cout << " random right " << population[ind_index][start_mst_x + 1][start_mst_y].num_children << std::endl;
	if (population[ind_index][start_mst_x][start_mst_y].up)std::cout << " random up " << population[ind_index][start_mst_x][start_mst_y - 1].num_children << std::endl;
	if (population[ind_index][start_mst_x][start_mst_y].down)std::cout << " random down " << population[ind_index][start_mst_x][start_mst_y + 1].num_children << std::endl;

	// creating segments
	r_edge_priority_que merge_que;
	int edge_candidate_count = 0;
	split_MST_segment(ind_index, que,merge_que);

	merge_small_segments(ind_index, merge_que, N_MERGE_SMALL_SEGMENT);

	
	//std::cout << "candidate edges size" << edge_candidates.size() << " count " << edge_candidate_count << std::endl;
	//std::cout << "entry_s size " << entry_s[ind_index].size() << std::endl;

	// setting entry value
	int total_segment_size = 0;
	for (auto it = entry_s[ind_index].begin(); it != entry_s[ind_index].end();) {
		//std::cout << " check " << it->x << " " << it->y << std::endl;
		int segment_size = set_start_segment_entry(*it, ind_index);
		if (segment_size == 0) {
			it = entry_s[ind_index].erase(it);
			//std::cout << " zero in size" << std::endl;
			continue;
		}
		else {
			//std::cout <<"segment " << it->x << " " << it->y << " " << segment_size << std::endl;
			it++;
		}
		total_segment_size += segment_size;

	}

	RGB invalid_color(-1, -1, -1);
	std::vector<pos>* edge_segments = edges_segment(ind_index);	
	for (int i = 0; i < entry_s[ind_index].size(); i++) {
		for (auto it = edge_segments[i].begin(); it != edge_segments[i].end(); ++it) {
			pos pos_entry = population[ind_index][it->x][it->y].entry;


			if (segment_prop[ind_index][pos_entry].avg_rgb == invalid_color ) {
				segment_prop[ind_index][pos_entry].avg_rgb = avg_rgb_seg(*this, ind_index, pos_entry);
				std::cout << i <<" adding color " << std::endl;
			}
			
			std::vector<edge> neigbours = get_neigbours(*it, ind_index, 0);
			for (auto xt = neigbours.begin(); xt != neigbours.end(); ++xt) {
				pos new_entry;
				if (xt->p1 == *it) {
					new_entry = population[ind_index][xt->p2.x][xt->p2.y].entry;
				}
				else {
					new_entry = population[ind_index][xt->p1.x][xt->p1.y].entry;
				}
				if (get_neighbor_dir(xt->p1, xt->p2) == SELF) std::cout << xt->p1.x << " " << xt->p1.y << " not neigh" << xt->p2.x << " " << xt->p2.y << std::endl;
				if (xt->p1.x > get_im_w() || xt->p2.x > get_im_w() || xt->p1.y > get_im_h() || xt->p2.y > get_im_h()) std::cout << " wrong " << std::endl;
				segment_prop[ind_index][pos_entry].borders[new_entry].push_back(*xt);
				//std::cout << "new entry " << new_entry.x << " " << new_entry.y << " size " << segment_prop[ind_index][pos_entry].borders[new_entry].size() << std::endl;
				if (std::find_if(segment_prop[ind_index][pos_entry].neighbour_entries.begin(), segment_prop[ind_index][pos_entry].neighbour_entries.end(), pos_comparator(new_entry)) == segment_prop[ind_index][pos_entry].neighbour_entries.end()) {
					segment_prop[ind_index][pos_entry].neighbour_entries.push_back(new_entry);
					//std::cout << " entry " << new_entry.x << " " << new_entry.y << " num_edges" << segment_prop[ind_index][pos_entry].neighbour_entries.size()  << std::endl;
				}
			}
		}
		//std::cout << " i " << i << std::endl;
	}

	
	std::cout << "total " << total_segment_size << "should be " << get_im_h()*get_im_w() << std::endl;

	//std::cout << "FINISHED " << std::endl;
	//std::cout << seconds - time(NULL) << std::endl;
	//seconds = time(NULL);
}

void Population::initialize_population() {
	
	for (int i = 0; i < 1;i++) {
		this->initialize_individual_PrimsMST(i);
		fitness_1.push_back(std::make_pair(0.0, i));
		fitness_2.push_back(std::make_pair(0.0, i));
		rank.push_back(std::make_pair(0, i));
		draw_segments_contour(i, i);
		cv::Mat im = draw_segments_black_contour(i);
		write_image_to_file(i, im);
	}
	int init_index = 0;
	for (int i = 1; i < N_IND+N_OFFSPRING; i++) {

		fitness_1.push_back(std::make_pair(0.0, i));
		fitness_2.push_back(std::make_pair(0.0, i));
		rank.push_back(std::make_pair(0, i));
		entry_s[i] = entry_s[init_index];
		edge_candidates[i] = edge_candidates[init_index];

		if (i >= N_IND) continue;
		segment_prop[i] = segment_prop[init_index];

		for (int x = 0; x < get_im_w(); x++) {
			for (int y = 0; y < get_im_h(); y++) {
				this->population[i][x][y].color        = this->population[init_index][x][y].color;
				this->population[i][x][y].left         = this->population[init_index][x][y].left;
				this->population[i][x][y].right        = this->population[init_index][x][y].right;
				this->population[i][x][y].up           = this->population[init_index][x][y].up;
				this->population[i][x][y].down         = this->population[init_index][x][y].down;
				this->population[i][x][y].entry.x      = this->population[init_index][x][y].entry.x;
				this->population[i][x][y].entry.y      = this->population[init_index][x][y].entry.y;
				this->population[i][x][y].num_children = this->population[init_index][x][y].num_children;
				this->population[i][x][y].parent_dir   = this->population[init_index][x][y].parent_dir;
			}
		}
	}

	// Calculating fitness and rank of individuals

	MOEA_fitness(*this, N_IND, entry_s, fitness_1, fitness_2);
	MOEA_rank(N_IND, rank, fitness_1, fitness_2);	
	cv::waitKey(0);

}

// NB NOT TESTED !!!!
// CHANGED CONSTRUCTOR TO ADD POPULATION SUCH THAT IT IS N_IND + N_OFFSPRING
void Population::MOEA_next_generation() {

	//TODO: TEST WHETHER OR NOT FITNESS 1 AND 2, AND RANK retains the correct order of the individuals
	
	// Select parents:
	int n_pop = N_IND; // what is this variable? what is the differnce n_pop and N_IND? Is it the current number in population?
	int tournament_size = 10;

	// find parentss
	std::vector<int> parents = rank_tournament_selection(*this, entry_s, n_pop, tournament_size, N_OFFSPRING, rank); // returns list of parent index
	for (int i = 0; i < N_OFFSPRING;) {
		//double rand_num = (rand() % 1000) / 1000.0;
		copy_individual(n_pop++, parents[i++]);
		//if (i >= N_OFFSPRING) break;
		//copy_individual(n_pop++, parents[i++]);
		//if (rand_num < CROSSOVER_RATE) {
		//	edge_candidates[n_pop - 2] = crossover_uniform_list_representation(*this, parents[i - 2], parents[i - 1], n_pop - 2);
		//	edge_candidates[n_pop - 1] = crossover_uniform_list_representation(*this, parents[i - 1], parents[i - 2], n_pop - 1);
		//}
	}
// mutations:
double mutation_rate = MUT_SPLIT_PERC + MUT_MERGE_PERC;
if (mutation_rate > 1.0) std::cout << " to high mutation rate" << std::endl;
for (int i = N_IND; i < n_pop; i++) {
	double rand_num = (rand() % 1000) / 1000.0;
	if (rand_num < MUTATION_RATE) {

		int num_mutations = rand() % 10;
		for (int j = 0; j < num_mutations; j++) {
			int attempts = 0;
			while (!mutation_greedy_merge_segments(*this, i) && 50 >= attempts++);
			if (attempts == 50) std::cout << "Failed to mutate child" << std::endl;
		}
	}
}

// Calculate fitness of offspring
for (int offspring_index = N_IND; offspring_index < N_IND + N_OFFSPRING; offspring_index++) {
	fitness_1[offspring_index].first = overall_deviation_ind(*this, offspring_index, entry_s[offspring_index]);
	fitness_1[offspring_index].second = offspring_index;
	fitness_2[offspring_index].first = edge_value_ind(*this, offspring_index, entry_s[offspring_index]);
	fitness_1[offspring_index].second = offspring_index;
}

MOEA_rank(n_pop, rank, fitness_1, fitness_2);
std::vector<int> survivors = NSGAII(*this, entry_s, n_pop, rank, fitness_1, fitness_2);
std::vector<int> non_survivors;
std::vector<int> survivors_offspring;

for (int i = 0; i < n_pop; i++) {
	if (find(survivors.begin(), survivors.end(), i) == survivors.end()) {
		if (i < N_IND) {
			non_survivors.push_back(i); // don't care if it is higher than N_IND
		}
	}
	else {
		if (i >= N_IND) {
			survivors_offspring.push_back(i);
		}
	}
}

//if (survivors_offspring.size() != non_survivors.size()) std::cout << "different size" << survivors_offspring.size() << " " << non_survivors.size() << std::endl;
//std::cin.get();
for (int i = 0; i < survivors_offspring.size(); i++) {
	//std::cout << " switch " << non_survivors[i] << " " << survivors_offspring[i] << std::endl;
	copy_individual(non_survivors[i], survivors_offspring[i]);

}

for (int i = 0; i < N_IND; i++) {
	std::cout << " rank " << rank[i].first << "," << rank[i].second << " fitness 1: " << fitness_1[i].first << "," << fitness_1[i].second << " fitness 2: " << fitness_2[i].first << "," << fitness_2[i].second << std::endl;
}

// return pareto rank 0 ? ( to main loop ? or ??)

}


std::vector<active_edge_t>& Population::get_edge_candidates(int ind_index) {
	return edge_candidates[ind_index];
}

void Population::merge_segments(int ind_index, edge merge_nodes) {
	//std::cout << merge_nodes.p1.x << "," << merge_nodes.p1.y << " p1, " << merge_nodes.p2.x << "," << merge_nodes.p2.y << " p2" << std::endl;
	//pos entry1 = population[ind_index][merge_nodes.p1.x][merge_nodes.p1.y].entry;
	//pos entry2 = population[ind_index][merge_nodes.p2.x][merge_nodes.p2.y].entry;
	//std::cout << entry1.x << "," << entry1.y << " entry1, " << entry2.x << "," << entry2.y << " entry 2" << std::endl;
	//std::cout << get_n_segment(entry1, ind_index, 0) << ", segment p1 size before merge" << std::endl;
	//std::cout << get_n_segment(entry2, ind_index, 0) << ", segment p2 size before merge" << std::endl;
	merge_segment_properties(ind_index, merge_nodes.p1, merge_nodes.p2);
	set_segment_entry(merge_nodes.p2, merge_nodes.p1, ind_index);
	set_dir_edge(merge_nodes.p1, merge_nodes.p2, 1, ind_index);
	//std::cout << get_n_segment(entry1, ind_index, 0) << ", segment p1 size after merge" << std::endl;
	//std::cout << get_n_segment(entry2, ind_index, 0) << ", segment p2 size after merge" << std::endl;
}

void Population::merge_segments(int ind_index, int edge_index, edge merge_nodes) {
	merge_segments(ind_index, merge_nodes);
	edge_candidates[ind_index][edge_index].active = 1;
}

void Population::merge_segment_properties(int ind_index, pos first, pos second) {
	static int called = 0;
	std::cout << called++ << std::endl;
	pos first_entry = population[ind_index][first.x][first.y].entry;
	pos second_entry = population[ind_index][second.x][second.y].entry;

	seg_prop_t *first_prop = &segment_prop[ind_index][first_entry];
	seg_prop_t *second_prop = &segment_prop[ind_index][second_entry];

	// Calculate segment properties of the combined segment
	first_prop->avg_rgb = (first_prop->avg_rgb + second_prop->avg_rgb) / 2.0;

	first_prop->borders.erase(second_entry);


	// delete from neigbour array
	for (std::vector<pos>::iterator it = first_prop->neighbour_entries.begin(); it != first_prop->neighbour_entries.begin(); ++it) {
		if (*it == second_entry) {
			first_prop->neighbour_entries.erase(it); // must erase from this array
		}
	}

	// set in edges

	for (std::map<pos, std::vector<edge>>::iterator it = second_prop->borders.begin(); it != second_prop->borders.end(); ++it) {
		if (it->first.x == first_entry.x && it->first.y == first_entry.y) {
			continue;
		}
		else if (it->first.x == second_entry.x && it->first.y == second_entry.y){
			std::cout << "hmm.... second = it " << std::endl;
			continue;
		}
		else {
			//std::cout << " size " <<  it->second.size() << std::endl;
			int j = 0;
			for (int i = 0; i < it->second.size(); i++) {
				
				if (get_neighbor_dir(it->second[i].p1, it->second[i].p2) == SELF) {
					std:cout << it->second[i].p1.x << "," << it->second[i].p1.y << " og " << it->second[i].p2.x << "," << it->second[i].p2.y << std::endl;
					std::cout << j << " i " << i <<std::endl;
					std::cout << second_prop->borders[it->first].size() << std::endl;
					std::cin.get();
				}
				if (i >= it->second.size()) {
					std::cout << i << " vs. size " << std::endl;
					
				}
				first_prop->borders[it->first].push_back(it->second[i]);
				j++;
			}
			//std::cout << first_prop->borders[it->first].size() << ", resultant size" << std::endl;
		}
	}

	for (auto it = second_prop->neighbour_entries.begin(); it != second_prop->neighbour_entries.begin(); ++it) {
		if (*it != first && std::find_if(segment_prop[ind_index][first_entry].neighbour_entries.begin(), segment_prop[ind_index][first_entry].neighbour_entries.end(), pos_comparator(*it)) != segment_prop[ind_index][first_entry].neighbour_entries.end()) {
			first_prop->neighbour_entries.push_back(*it);
		}
	}

	segment_prop[ind_index].erase(second_entry);
	std::cout << " get size of merged segment " << get_n_segment(first_entry, ind_index,0) + get_n_segment(second, ind_index,0) << std::endl;

}

void Population::split_segment(int ind_index, int edge_index, edge split_nodes) {
	set_dir_edge(split_nodes.p1, split_nodes.p2, 0, ind_index);
	set_segment_entry(split_nodes.p1, split_nodes.p1, ind_index);
	edge_candidates[ind_index][edge_index].active = 0;
}


void Population::set_dir_edge(pos& parent, pos& child, int on, int ind_index) {
	int x1 = parent.x;
	int x2 = child.x;
	int y1 = parent.y;
	int y2 = child.y;
	//std::cout << " x1 " << x1 << " y1 " << y1 << " x2" << x2 << " y2 " << y2 << std::endl;
	if (x1 > x2) {
		population[ind_index][x1][y1].left = on;
		population[ind_index][x2][y2].right = on;
		//std::cout << "Combining" << std::endl;
	}
	else if (x1 < x2) {
		population[ind_index][x1][y1].right = on;
		population[ind_index][x2][y2].left = on;
		//std::cout << "Combining" << std::endl;

	}
	else if (y1 > y2) {
		population[ind_index][x1][y1].up = on;
		population[ind_index][x2][y2].down = on;
		//std::cout << "Combining" << std::endl;

	}
	else {
		population[ind_index][x1][y1].down = on;
		population[ind_index][x2][y2].up = on;
		//std::cout << "Combining" << std::endl;

	}
}

void Population::set_dir_edge_and_parent(pos& parent, pos& child, int on, int ind_index) {
	int x1 = parent.x;
	int x2 = child.x;
	int y1 = parent.y;
	int y2 = child.y;
	//std::cout << " x1 " << x1 << " y1 " << y1 << " x2" << x2 << " y2 " << y2 << std::endl;
	
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

int Population::set_start_segment_entry(pos& entry, int ind_index) {
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

int Population::set_segment_entry(pos& entry, pos& set, int ind_index) {
	stack<pos> branch_points;
	pos invalid_pos(-1, -1);
	//std::cout << "Merging" << std::endl;
	//std::cout << "you not taken " << std::endl;
	population[ind_index][entry.x][entry.y].entry = set;
	pos next = entry;
	int count = 0;
	while (next.x != static_cast<unsigned short>(-1)) {
		population[ind_index][next.x][next.y].entry = set;
		next = traverse_ST(*this, ind_index, next, branch_points);
		count++;
	}
	if (count == 0) population[ind_index][entry.x][entry.y].entry = invalid_pos;
	remove_color(*this, ind_index, entry, branch_points);
	return count;
}

int Population::get_n_segment(pos& entry, int ind_index, bool check_for_taken) {
	stack<pos> branch_points;
	
	//std::cout << entry.x << "," << entry.y << " entry in get_n_segment" << std::endl;

	pos invalid_pos(-1, -1);
	if (population[ind_index][entry.x][entry.y].entry != invalid_pos && check_for_taken) {
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

std::vector<pos> Population::get_segment_entries(int ind_index) {
	return entry_s[ind_index];
}

seg_prop_t Population::get_segment_property(int ind_index, pos seg_entry) {
	return segment_prop[ind_index][seg_entry];
}

bool Population::get_pos_within_borders(pos& p) {
	return (p.x >= 0 && p.x < get_im_w() && p.y >= 0 && p.y < get_im_h());
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
	//std::cout << " test draw segments " << std::endl;
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
		//std::cout << count << " " <<it->x << " "<< it->y << " " << count_pixels << std::endl;
		count++;
		if (count == 10) count = 0;
	}
	//std::cout << "show segment" << std::endl;
	cv::namedWindow("image", 1);
	//std::cout << "show image" << std::endl;
	cv::imshow("image", segment);                   
	//cv::waitKey(0);
}

cv::Mat Population::draw_segments_black_contour(int ind_index) {

	cv::Mat image(get_im_h(), get_im_w(), CV_8UC3, cv::Scalar(255, 255, 255));

	std::vector<pos>* edge_segment = edges_segment(ind_index);
	RGB color(0, 0, 0); // black
	
	std::cout << " test contour plot imshow " << std::endl;
	for (int i = 0; i < entry_s[ind_index].size(); i++) {
		for (auto it = edge_segment[i].begin(); it != edge_segment[i].end(); ++it) {
			image.at<cv::Vec3b>(it->y, it->x)[2] = color.r;
			image.at<cv::Vec3b>(it->y, it->x)[1] = color.g;
			image.at<cv::Vec3b>(it->y, it->x)[0] = color.b;
		}
	}
	delete[] edge_segment;
	//std::string window_name = "contour " + to_string(name);
	//cv::namedWindow(window_name, 1);
	//cv::imshow(window_name, segment);
	return image;
}

void Population::draw_segments_contour(int ind_index, int name) {
	cv::Mat segment = cv::imread(img_path, 1);

	std::vector<pos>* edge_segment = edges_segment(ind_index);
	RGB color(0, 255, 0); // green ?=

	std::cout << " test contour plot imshow " << std::endl;
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
	std::string window_name = "contour " + to_string(name);
	cv::namedWindow(window_name, 1);
	//std::cout << "show image" << std::endl;
	cv::imshow(window_name, segment);
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
		int cout = 0;
		/*if (next.x == 35 && next.y == 317) {
			std::cout << " in segment " << std::endl;
			cout = 1;
		}
		else {
			cout = 0;
		}*/
		while (next.x != static_cast<unsigned short>(-1)) {
			if (check_if_edge(next,ind_index,cout) == 1) {
				segment[count].push_back(next);
				//std::cout << " new edge" << std::endl;
			}
			next = traverse_ST(*this, ind_index, next, branch_points);
			count_pixels++;
		}
		//std::cout << it->x << " " <<it->y << " "<< count_pixels << " edges " << segment[count].size() <<std::endl;
		count++;
		remove_color(*this, ind_index, *it, branch_points);
	}
	return segment;
}

int Population::split_MST_segment(int ind_index, edge_priority_que& que, r_edge_priority_que& merge_que) {
	int segment_size = SMALL_SEGMENT_SIZE;
	int n_segments = N_SMALL_SEGMENT;

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
			set_dir_edge_and_parent(parent, child, 0,ind_index);
			//std::cout << "n_pixels " << population[ind_index][child.x][child.y].num_children;
			//std::cout << " get_ parent " << get_n_segment(parent, ind_index, 1) << " calculate " << n_segment_2 - n_segment << std::endl;
			//std::cout << " get_ child " << get_n_segment(child, ind_index, 1) << " calculate " <<  n_segment << std::endl;
			change_parents_n_segment(parent, child, ind_index);
			//int min_pixels = get_n_segment(child, ind_index, 1);
			//std::cout << " get_n " << min_pixels << std::endl;
			entry_s[ind_index].push_back(child);
			entry_s[ind_index].push_back(parent);
			edge_candidates[ind_index].push_back({ temp, false });
			merge_que.push(temp);
			n_segments--;
		}
	}
	return n_segments;
}

int Population::merge_small_segments(int ind_index, r_edge_priority_que& que, int n_segments) {


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
		
		set_dir_edge_and_parent(parent, child, 1, ind_index);

		// MUST BE DONE change_parents_n_segment(parent, child, ind_index);

		entry_s[ind_index].push_back(child);
		entry_s[ind_index].push_back(parent);
		n_segments--;
	}
	return n_segments;
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

std::vector<edge> Population::get_neigbours(pos curr, int ind_index, int cout) {

	std::vector<edge> temp;

	if (population[ind_index][curr.x][curr.y].left == 0 && curr.x - 1 > 0) {
		pos neighbour = curr + LEFT;
		//pos temp = population[ind_index][neighbour.x][neighbour.y].entry;
		//if (cout) std::cout << population[ind_index][curr.x][curr.y].entry.x <<" "<< population[ind_index][curr.x][curr.y].entry.x <<" " << temp.x << " " <<temp.y << std::endl;
		if (population[ind_index][neighbour.x][neighbour.y].entry != population[ind_index][curr.x][curr.y].entry) {
			//	if (cout) std::cout << " new edge" << std::endl;
			edge new_edge(curr, neighbour, dist(get_RGB(curr),get_RGB(neighbour)));
			temp.push_back(new_edge);
		}
	}

	if (population[ind_index][curr.x][curr.y].right == 0 && curr.x + 1 < get_im_w()) {
		pos neighbour = curr + RIGHT;
		//pos temp = population[ind_index][neighbour.x][neighbour.y].entry;
		//if (cout) std::cout << population[ind_index][curr.x][curr.y].entry.x << " " << population[ind_index][curr.x][curr.y].entry.x << " " << temp.x << " " << temp.y << std::endl;
		if (population[ind_index][neighbour.x][neighbour.y].entry != population[ind_index][curr.x][curr.y].entry) {
			edge new_edge(curr, neighbour, dist(get_RGB(curr), get_RGB(neighbour)));
			temp.push_back(new_edge);
		}
	}

	if (population[ind_index][curr.x][curr.y].up == 0 && curr.y - 1 > 0) {
		pos neighbour = curr + UP;
		if (population[ind_index][neighbour.x][neighbour.y].entry != population[ind_index][curr.x][curr.y].entry) {
			edge new_edge(curr, neighbour, dist(get_RGB(curr), get_RGB(neighbour)));
			temp.push_back(new_edge);
		}
	}

	if (population[ind_index][curr.x][curr.y].down == 0 && curr.y + 1 < get_im_h()) {
		pos neighbour = curr + DOWN;
		if (population[ind_index][neighbour.x][neighbour.y].entry != population[ind_index][curr.x][curr.y].entry) {
			edge new_edge(curr, neighbour, dist(get_RGB(curr), get_RGB(neighbour)));
			temp.push_back(new_edge);
		}
	}

	return temp;

}

void Population::copy_individual(int l_index, int r_index) {
	for (int x = 0; x < get_im_w(); x++) {
		for (int y = 0; y < get_im_h(); y++) {
			this->population[l_index][x][y].color = this->population[r_index][x][y].color;
			this->population[l_index][x][y].left = this->population[r_index][x][y].left;
			this->population[l_index][x][y].right = this->population[r_index][x][y].right;
			this->population[l_index][x][y].up = this->population[r_index][x][y].up;
			this->population[l_index][x][y].down = this->population[r_index][x][y].down;
			this->population[l_index][x][y].entry.x = this->population[r_index][x][y].entry.x;
			this->population[l_index][x][y].entry.y = this->population[r_index][x][y].entry.y;
			this->population[l_index][x][y].num_children = this->population[r_index][x][y].num_children;
			this->population[l_index][x][y].parent_dir = this->population[r_index][x][y].parent_dir;
		}
	}
	entry_s[l_index] = entry_s[r_index];
	edge_candidates[l_index] = edge_candidates[r_index];
	fitness_1[l_index].first = fitness_1[r_index].first;
	fitness_2[l_index].first = fitness_2[r_index].first;
	rank[l_index].first = rank[r_index].first;
	segment_prop[l_index] = segment_prop[r_index];
}

void Population::draw_pareto_front() {
	for (int i = 0; i < N_IND; i++) {
		if (rank[i].first == 0) {
			cv::Mat img = draw_segments_black_contour(rank[i].second);
			write_image_to_file(rank[i].second, img);
			//std::cout << " rank . second " << rank[i].second << std::endl;
		}
	}
}
