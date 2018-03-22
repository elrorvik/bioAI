#include"bee.h"

void bee_algorithm(Operation_manager &om) {
	
	// TODO: redo this - adaptive amount of employed or no?
	const int NUM_SCOUTS = 5;
	const int NUM_EMPLOYED = 5;
	const int NUM_ONLOOKER_PER_EMPLOYED = 5;
	const int NUM_BEES = NUM_SCOUTS + NUM_EMPLOYED * (1 + NUM_ONLOOKER_PER_EMPLOYED);
	int num_scouts_recruited = 0;

	// Initialize bee vector
	std::vector<std::vector<int>> bees;
	bees.emplace_back();
	/*for (int j = 0; j < om.get_n_machines(); j++) {
		for (int k = 0; k < om.get_n_jobs(); k++) {
			bees[i].push_back(j);
		}
	}*/
	for (int i = 1; i < NUM_BEES; i++) {
		bees[i] = bees[0];
	}

	// Search algorithm
	while (1) {
		// Random scout bees

		// Recruit employed bees

		// Recruit onlooker bees

		// Swap best employed with its best owned onlooker

		// If stagnation, store achieved result of employed bee, and turn into scout bee

	}
}