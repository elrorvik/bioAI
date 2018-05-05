#include <iostream>
#include <time.h>
#include "operation_manager.h"
#include "BCA.h"
#include "ACO.h"

int main() {
	srand(time(0));
	std::string filename = "test_data\\2.txt";
	Operation_manager om(filename);
	om.test();

	//ACO(om); // bug in program
	bee_colony_algorithm(om);
	std::cout << "finished" << std::endl;
	
	system("pause");
	return 0;
}