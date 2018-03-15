
#include "global.h"
#include "population.h"
#include "var_operators.h"
#include "fitness.h"
#include <iostream>
//#include<vector>


std::vector<active_edge_t> crossover_uniform_list_representation(Population &p, int parent_A, int parent_B, int offspring_index) {
	std::vector<active_edge_t> parent_A_chromosome = p.get_edge_candidates(parent_A);
	std::vector<active_edge_t> parent_B_chromosome = p.get_edge_candidates(parent_B);

	std::vector<active_edge_t> offspring_chromosome = parent_A_chromosome;
	for (int gene_index = 0; gene_index < parent_A_chromosome.size(); gene_index++) {
		bool from_B = rand() % 2;
		if (from_B) {
			offspring_chromosome[gene_index] = parent_B_chromosome[gene_index];
			if (offspring_chromosome[gene_index].active != parent_A_chromosome[gene_index].active) {
				if (offspring_chromosome[gene_index].active) p.merge_segments(offspring_index, gene_index, offspring_chromosome[gene_index].edge);
				else p.split_segment(offspring_index, gene_index, offspring_chromosome[gene_index].edge);
			}
		}
	}
	return offspring_chromosome;
}

int crossover_merge(Population &p, int parent_index, int offspring_index) {
	std::vector<pos> seg_entries = p.get_segment_entries(offspring_index);
	int rand_num_segments = (seg_entries.size() / 10) + rand() % seg_entries.size() / 4;
	int i = 0;
	bool any_segment_merged = false; // For return value

	// Merge segments in the offspring if the parent doesn't have the border, and if chance allows it
	while (rand_num_segments - i >= 0) {
		int entry_index = rand() % seg_entries.size();
		seg_prop_t segment_properties = p.get_segment_property(offspring_index, seg_entries[entry_index]);
		bool segment_merged = false;
		for (auto segment_it = segment_properties.neighbour_entries.begin(); segment_it != segment_properties.neighbour_entries.end(); ++segment_it) {
			if (0.5 > (rand() % 1000) / 1000.0) continue; // By chance, skip this neighbour even if they can be merged

			// Check if parent has this border. If not, merge.
			for (auto edge_it = segment_properties.borders[*segment_it].begin(); edge_it != segment_properties.borders[*segment_it].end(); ++edge_it) {
				pos entry1_parent = p.get_node(parent_index, edge_it->p1)->entry;
				pos entry2_parent = p.get_node(parent_index, edge_it->p2)->entry;

				// If the segment border edge in the offspring isn't a border edge in the parent: merge
				if (entry1_parent.x != entry2_parent.x || entry1_parent.y != entry2_parent.y) {
					p.merge_segments(offspring_index, *edge_it);
					segment_merged = true;
					any_segment_merged = true;
					break;
				}
			}
			if (segment_merged) break;
		}

	}
	return any_segment_merged;
}

int  mutation_split_segments(Population &p, int ind_index) {
	std::vector<active_edge_t> chromosome = p.get_edge_candidates(ind_index);
	int loci = rand() % chromosome.size();

	int count = 10;
	while (chromosome[loci].active == 0 && count > 0) {
		loci = rand() % chromosome.size();
		count--;
	}
	if (chromosome[loci].active == 1) {
		p.split_segment(ind_index, loci, chromosome[loci].edge);
		return 1;
	}
	else {
		return 0;
	}
}

int  mutation_merge_segments(Population &p, int ind_index) {
	std::vector<active_edge_t> chromosome = p.get_edge_candidates(ind_index);
	int loci = rand() % chromosome.size();

	int count = 10;
	while (chromosome[loci].active == 1 && count > 0) {
		loci = rand() % chromosome.size();
		count--;
	}
	if (chromosome[loci].active == 0) {
		p.merge_segments(ind_index, loci, chromosome[loci].edge);
		return 1;
	}
	else {
		return 0;
	}
}


int mutation_greedy_merge_segments(Population &p, int ind_index) {
	std::vector<pos> seg_entries = p.get_segment_entries(ind_index);
	int entry_index = rand() % seg_entries.size();
	seg_prop_t segment_properties = p.get_segment_property(ind_index, seg_entries[entry_index]);
	
	for (int i = 0; i < segment_properties.neighbour_entries.size(); i++) {

		segment_properties = p.get_segment_property(ind_index, seg_entries[entry_index]);
		if (segment_properties.neighbour_entries.size() <= i) {
			std::cout << " to short list " << std::endl;
			return 0;
		}
		//std::cout <<"segment_neighbours size " <<  i <<" " << segment_properties.neighbour_entries.size() << std::endl;
		pos entry_other = segment_properties.neighbour_entries[i];
		//std::cout << " Merge " << entry_other.x << entry_other.y << " and " << seg_entries[entry_index].x << " " << seg_entries[entry_index].y << std::endl;
		edge min_edge = { { -1, -1 },{ -1, -1 }, DBL_MAX };

		// Calculate avg rgb dist in between segments
		RGB avg_rgb_other = p.get_segment_property(ind_index, entry_other).avg_rgb;
		double avg_RGB_dist = dist(segment_properties.avg_rgb, avg_rgb_other); // eucledian distnace avg RGB
		if (avg_rgb_other.r > 255 || avg_rgb_other.g > 255 || avg_rgb_other.b > 255) {
			std::cout << "illegal RGB" << std::endl;
			std::cout << avg_rgb_other.r << ", " << avg_rgb_other.g << ", " << avg_rgb_other.b << std::endl;
			std::cout << segment_properties.avg_rgb.r << ", " << segment_properties.avg_rgb.g << ", " << segment_properties.avg_rgb.b << std::endl;
			std::cin.get();
		}

		// Calculate avg rgb dist in border of segments
		double neighbour_border_dist = 0;
		const std::vector<edge> border_edges = segment_properties.borders[entry_other];

		//std::cout << "size " << border_edges.size() << std::endl;
		for (int j = 0; j < border_edges.size(); j++) {
			neighbour_border_dist += border_edges[j].RGBdist;
			if (min_edge.RGBdist > border_edges[j].RGBdist) {
				min_edge = border_edges[j];
				//std::cout << p.get_node(ind_index, min_edge.p1)->entry.x << " " << p.get_node(ind_index, min_edge.p1)->entry.y << std::endl;
				//std::cout << p.get_node(ind_index, min_edge.p2)->entry.x << " " << p.get_node(ind_index, min_edge.p2)->entry.y << std::endl;
				//std::cout << p.get_node(ind_index, min_edge.p1)->entry.x - p.get_node(ind_index, min_edge.p2)->entry.x << ", " << p.get_node(ind_index, min_edge.p1)->entry.y - p.get_node(ind_index, min_edge.p2)->entry.y << ", neighbor status" << std::endl;
			}
			if (!(abs(min_edge.p1.x - min_edge.p2.x) <= 1 || abs(min_edge.p1.y - min_edge.p2.y) <= 1)) std::cout << "Different" << std::endl;
		}
		neighbour_border_dist /= border_edges.size();

		//std::cout << "Can i activate? : " << avg_RGB_dist << " " <<  neighbour_border_dist << std::endl;
		if (avg_RGB_dist  < AVG_RGB_SEG_DEST_CRITERIA && neighbour_border_dist < AVG_RGB_BORDER_DEST_CRITERIA) {

			//std::cout << min_edge.p1.x << " " << min_edge.p1.y << " <-> " << min_edge.p2.x << " " << min_edge.p2.y << std::endl;
			//std::cout << p.get_node(ind_index, min_edge.p1)->entry.x << " " << p.get_node(ind_index, min_edge.p1)->entry.y << std::endl;
			//std::cout << p.get_node(ind_index, min_edge.p2)->entry.x << " " << p.get_node(ind_index, min_edge.p2)->entry.y << std::endl << std::endl;
			//std::cin.get();
			//std::cout << "Merge, activate!" << std::endl;
			p.merge_segments(ind_index, min_edge);
			return 1;
		}

	}

	return 0;
}