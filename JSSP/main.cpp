#include <iostream>
#include <time.h>
#include "operation_manager.h"
#include "BCA.h"
#include "ACO.h"
#include "file.h"

int main() {
	srand(time(0));
	std::string filename = "test_data\\1.txt";
	Operation_manager om(filename);
	//om.print_operation_sequence();
	//om.test();

	//ACO(om); // bug in program
	//std::cout << std::endl << std::endl;
	bee_colony_algorithm(om, 0, true);
	write_file("plot\\sol.txt", om);
	
	std::cout << "finished" << std::endl;
	
	system("pause");
	return 0;
}