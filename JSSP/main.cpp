#include <iostream>
#include <time.h>
#include "operation_manager.h"
#include "BCA.h"

int main() {
	srand(time(0));
	std::string filename = "test_data\\2.txt";
	Operation_manager om(filename);

	bee_colony_algorithm(om);
	
	system("pause");
	return 0;
}