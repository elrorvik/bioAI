#pragma once
#include<string>
#include<set>

#include"mVRP.h"
using namespace std;

void read_data(const string filename, set<customer> &customers, set<depot> &depots, int &n_vehicles, int &n_customers, int &n_depots);