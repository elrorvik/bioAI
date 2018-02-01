#pragma once

struct customer {
	int index;
	int x;
	int y;
	int duration;
	int demand;
};

struct depot {
	int max_duration_per_vehicle;
	int max_load_per_vehicle;
	int x;
	int y;
};


void GA_mVRP();

const int invalid_city = -1;