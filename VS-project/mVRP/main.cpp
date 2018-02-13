#include <iostream>
#include "mVRP.h"
#include "visualization.h"
#include <ctime> 


int main(int num_args, char** arg_strings) {
	time_t now;
	time(&now);
	srand((unsigned int)now);

	std::cout << "p01 optimal value: " << 576.87 << std::endl;
	std::cout << "5% target: " << 576.87+ 576.87*0.05 << std::endl;
	std::cout << "10% target: " << 576.87 + 576.87*0.1 << std::endl;

	std::cin.get();

	GA_mVRP();

	system("pause");
	return 0;
}