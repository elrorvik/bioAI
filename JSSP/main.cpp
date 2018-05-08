#include <iostream>
#include <time.h>
#include "operation_manager.h"
#include "BCA.h"
#include "ACO.h"
#include "file.h"

int main() {
	srand(time(0));


	std::string filename = "test_data\\1.txt";
	float target = 56;
	filename = "test_data\\2.txt";
	target = 1059;
	/*filename = "test_data\\3.txt";
	target = 1276;
	filename = "test_data\\4.txt";
	target = 1130;
	filename = "test_data\\5.txt";
	target = 1451;
	filename = "test_data\\6.txt";
	target = 979;
	filename = "test_data\\7.txt";
	target = 1000; //944;
	filename = "test_data\\8.txt";
	target = 8279/1.1;*/
	std::cout << " 1.1 of target " << target * 1.1 << std::endl;
	Operation_manager om(filename);
	std::cout << " ******* ACO *********** " << std::endl;
	
	ant_coloy_optimization(om, target);

	std::cout << "\n ******* ABC *********** " << std::endl;
	bee_colony_algorithm(om, target, true);
	
	
	system("pause");
	return 0;
}