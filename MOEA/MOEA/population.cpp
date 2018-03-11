
	print_entry_properties(ind_index, 0);
	std::cout << "***************************************" << std::endl << std::endl;
	std::cin.get();
	auto it1 = std::find_if(first_prop->neighbour_entries.begin(), first_prop->neighbour_entries.end(), pos_comparator(second_entry));
	std::cout << "REMOVING: " << it1->x << "," << it1->y << " | Size: " << first_prop->neighbour_entries.size() << std::endl;
	first_prop->neighbour_entries.erase(it1);
	std::cout << "Size after delete: " << first_prop->neighbour_entries.size() << std::endl;

	std::cout << "*** after removing the annexed segment form neighbour_entries ***" << std::endl;
	print_entry_properties(ind_index, 0);
	std::cout << "*****************************************************************" << std::endl << std::endl;
	std::cin.get();

	// Update other's borders and neighbours to reflect the annexation
	for (auto it = entry_s[ind_index].begin(); it != entry_s[ind_index].end(); ++it) {

		auto xt = std::find_if(segment_prop[ind_index][*it].neighbour_entries.begin(), segment_prop[ind_index][*it].neighbour_entries.end(), pos_comparator(second_entry));
		if (xt != segment_prop[ind_index][*it].neighbour_entries.end()) {
			segment_prop[ind_index][*it].neighbour_entries.erase(xt);
		}

		if (*it == first_entry) {
			continue;
		}

		for (auto yt = segment_prop[ind_index][*it].borders[second_entry].begin(); yt != segment_prop[ind_index][*it].borders[second_entry].end(); ++yt) {
			segment_prop[ind_index][*it].borders[first_entry].push_back(*yt);
		}
		segment_prop[ind_index][*it].borders.erase(second_entry);

		auto zt = std::find_if(segment_prop[ind_index][*it].neighbour_entries.begin(), segment_prop[ind_index][*it].neighbour_entries.end(), pos_comparator(first_entry));
		if (zt == segment_prop[ind_index][*it].neighbour_entries.end()) {
			segment_prop[ind_index][*it].neighbour_entries.push_back(first_entry);
		}
	std::cout << "*** after removing segment from segment_prop - DONE DONE DONE ***" << std::endl;
	print_entry_properties(ind_index, 0);
	std::cout << "*****************************************************************" << std::endl << std::endl;
	std::cin.get();
	/*int count = 0;
		count = 0;
	}*/