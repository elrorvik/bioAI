#include<vector>
#include"operation_manager.h"

typedef std::vector<std::vector<std::vector<float>>> vec_float_3d;
typedef std::vector<std::vector<float>> vec_float_2d;
typedef std::vector<float> vec_float_1d;
const int PHERO_INIT = 10;

void ACO(Operation_manager& om);
void initialize_pheromone(Operation_manager& om, vec_float_3d& phi, vec_float_3d& delta_phi);