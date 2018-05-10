#pragma once
#include "operation_manager.h"
#include <vector>


const float RHO  = 0.7;
const float PHERO_INIT = 0.1; //  max value at init 
const float PHERO_LIMIT_MAX = 0.5;
const float PHERO_LIMIT_MIN = 0.01;
const float Q = 100;

void ant_coloy_optimization(Operation_manager& om, float target);
