#pragma once
#include<vector>
#include <opencv2/highgui/highgui.hpp>

struct pos;
struct node;
struct RGB;

class Population{
private:
	cv::Mat im;
	node ***population;
	std::vector<pos> * segment_nr;
	int n_segments;
public:
	Population();
	~Population();
	void initialize_k_means();
	RGB get_RGB(int y, int x);
	int get_im_w();
	int get_im_h();
	pos get_pixel_segment(int x, int y, int individual);
};

