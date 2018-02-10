#include <iostream>
#include "mVRP.h"
#include "visualization.h"
#include <ctime> 


int main(int num_args, char** arg_strings) {
	srand(time(0));
	GA_mVRP();

	system("pause");
	return 0;
}