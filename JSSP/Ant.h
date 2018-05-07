#pragma once
#include "operation_manager.h"
#include <vector>

//#define ALPHA 0.2
//#define BETHA 0.8
#define RHO 0.7
#define PHERO_INIT 0.1 //Max value at init
#define PHERO_MAX 0.5 //Max value 
#define PHERO_MIN 0.01 //Min value
#define Q 100

struct Interval{
	float start;
	float end;
	bool isInIntervall(float val) {
		return (val >= start && val <= end);
	}
	int index;
};

void ants(Operation_manager& om, float target);
