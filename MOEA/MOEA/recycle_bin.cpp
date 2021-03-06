
/*void Population::initialize_population_PrimsMST() {

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

	// Test code
	//node **graph;
	//graph = new node*[3];
	//for (int i = 0; i < 3; i++) {
	//graph[i] = new node[3];
	//}
	//int height = 3;
	//int width = 3;

	//minHeap.emplace(edge_information{ UP, 2, 2, 10 });
	//minHeap.emplace(edge_information{ UP, 2, 2, -2.2 });
	//minHeap.emplace(edge_information{ DOWN, 3, 2, 3 });
	//std::cout << minHeap.top().RGBdist << std::endl;
	//std::cout << "Her!" << std::endl;
	//std::cin.get();

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
			//std::cin.get();
			functor.add_uncolored_edges_to_heap(minHeap, *this, ind_index, entry);
		}
		unsigned int num_colored = 1;
		while (num_colored < get_im_w()*get_im_h()) {
			//while (!minHeap.empty()) {
			//std::cout << minHeap.top().RGBdist << std::endl;
			//minHeap.pop();
			//}
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
}/*

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