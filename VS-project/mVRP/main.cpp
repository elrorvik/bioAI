#include <iostream>
#include "mVRP.h"
#include "visualization.h"
#include <ctime> 


int main(int num_args, char** arg_strings) {
	time_t now;
	time(&now);
	srand((unsigned int)now);

	GA_mVRP();

	system("pause");
	return 0;
}