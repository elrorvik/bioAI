#pragma once
struct node;
#ifndef FITNESS_H
#define FITNESS_H
struct RGB;

double dist(RGB p1, RGB p2);
double overall_deviation(node segment_entry);

#endif