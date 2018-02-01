#pragma once
#include "mVRP.h"


void initialize_population(struct costumer population, int n_vehicles, int n_customers, int n_depots, customer ** costumers, std::set<depot> &depots);