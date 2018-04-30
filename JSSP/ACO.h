#pragma once
#include<vector>
#include"operation_manager.h"


const int a = 1; // pheormone updating counter
const int b = 0; // step counter
const int t = 1;
const double ALPHA = 0.001;
const double BETHA = 0.001; // need to change value
const double Q = 0.1; //
const double RHO = 0.001;
const double PHERO_MIN = 0.001;
const double PHERO_MAX = 0.001;

typedef std::vector<std::vector<std::vector<float>>> vec_float_3d;
typedef std::vector<std::vector<float>> vec_float_2d;
typedef std::vector<float> vec_float_1d;


void ACO(Operation_manager& om);
void initialize_pheromone(Operation_manager& om, vec_float_3d& phi, vec_float_3d& delta_phi,int &total_opeartions);
void print_vector(vec_float_3d& vector);
double eta(Operation_manager& tabu, int job);

void update_delta_phi(std::vector<std::vector<int>> &colony, std::vector<double> &colony_finish_time, vec_float_3d& delta_phi, int ant_i, int m, int n);


