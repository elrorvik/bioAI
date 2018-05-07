#include <iostream>
#include <time.h>
#include "operation_manager.h"
#include "BCA.h"
#include "Ant.h"
#include "file.h"

int main() {
	srand(time(0));


	std::string filename = "test_data\\1.txt";
	float target = 56;
	filename = "test_data\\2.txt";
	target = 1059;
	filename = "test_data\\3.txt";
	target = 1276;
	filename = "test_data\\4.txt";
	target = 1130;
	filename = "test_data\\5.txt";
	target = 1451;
	filename = "test_data\\6.txt";
	target = 979;
	filename = "test_data\\7.txt";
	target = 944;
	/*std::string filename = "test_data\\8.txt";
	float target = 8200/1.1;*/
	Operation_manager om(filename);

	ants(om,target);
	
	//std::cout << std::endl << std::endl;
	//bee_colony_algorithm(om, target, true);
	write_file("plot\\sol.txt", om);
	
	std::cout << "finished" << std::endl;
	
	system("pause");
	return 0;
}