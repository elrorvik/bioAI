#include <iostream>
#include <time.h>
#include "operation_manager.h"
#include "BCA.h"
#include "Ant.h"

int main() {
	srand(time(0));


	std::string filename = "test_data\\7.txt";
	float target = 944;
	Operation_manager om(filename);

	ants(om,target);
	
	//std::cout << std::endl << std::endl;
	bee_colony_algorithm(om, 0, true);
	write_file("plot\\sol.txt", om);
	
	std::cout << "finished" << std::endl;
	
	system("pause");
	return 0;
}