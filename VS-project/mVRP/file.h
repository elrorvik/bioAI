#pragma once
#include<string>
#include<set>
#include<fstream>
using namespace std;

extern bool test_ONOFF;

struct customer {
    int index;
    int x;
    int y;
    int duration;
    int demand;

    customer(int index, int x, int y, int duration, int demand) : index(index), x(x), y(y), duration(duration), demand(demand) {}

};
bool operator<(const customer &right, const customer &left) {
    return (right.index < left.index);
}

struct depot{
    int max_duration_per_vehicle;
    int max_load_per_vehicle;
    int x;
    int y;

    depot(int max_duration_per_vehicle, int max_load_per_vehicle, int x, int y)
        : max_duration_per_vehicle(max_duration_per_vehicle), max_load_per_vehicle(max_load_per_vehicle), x(x), y(y) {}
};

bool operator<(const depot &right, const depot &left) {
    return (right.x + right.y < left.x + left.y);
}

void fetch_data(const string filename, set<customer> &customers, set<depot> &depots, int &n_vehicles, int &n_customers, int &n_depots);