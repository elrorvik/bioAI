#pragma once

struct customer {
    int index = 0;
    int x = 0;
    int y = 0;
    int duration = 0;
    int demand = 0;

    customer(int index, int x, int y, int duration, int demand) : index(index), x(x), y(y), duration(duration), demand(demand) {}
	customer() {}

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


void GA_mVRP();

const int invalid_city = -1;