#pragma once
#ifndef FITNESS_H
#define FITNESS_H
struct RGB;
struct node;

double dist(RGB p1, RGB p2);
double overall_deviation(node segment_entry);

#endif