#include <iostream>
#include <time.h>
#include "operation_manager.h"
#include "BCA.h"
#include "Ant.h"
#include "file.h"

int main() {
	srand(time(0));


	std::string filename = "test_data\\8.txt";
	float target = 8200/1.1;
	Operation_manager om(filename);

	//ants(om,target);
	
	//std::cout << std::endl << std::endl;
	bee_colony_algorithm(om, target, true);
	
	std::cout << "finished" << std::endl;
	
	system("pause");
	return 0;
}