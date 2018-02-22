#include<iostream>
#include"global.h"
#include"population.h"


// opencv 
#include <opencv2/highgui/highgui.hpp>


Population::~Population() {

}

Population::Population() {

	this->im = cv::imread(img_path, 1);   // Read the file

	if (!im.data)                              // Check for invalid input
	{
		std::cout << "Could not open or find the image" << std::endl;
		exit(0);
	}

	int im_h = im.rows;
	int im_w = im.cols;
	population = new node**[N_IND];
	for (int ind_i = 0; ind_i < N_IND; ind_i++) {
		population[ind_i] = new node*[im_w];
		for (int row_i = 0; row_i < im_w; row_i++) {
			population[ind_i][row_i] = new node[im_h]{};
		}
	}
	
	
}

RGB Population::get_RGB(int x , int y) {
	int r = im.at<cv::Vec3b>(y, x)[2];
	int g = im.at<cv::Vec3b>(y, x)[1];
	int b = im.at<cv::Vec3b>(y, x)[0];
	return RGB(r,g, b);
}

int Population::get_im_w() {
	return this->im.cols;
}
int Population::get_im_h() {
	return this->im.rows;
}

pos Population::get_pixel_segment(int x,int y, int individual) {
	return this->population[individual][x][y].entry;
}

void Population::initialize_k_means() {
	// Kruskal's =~ Prim's?
	// Remove the two nodes with least edge weight (weight = "cost" = RGB distance) and add it to its own tree

	
}

/*void Population::read_image(Mat* img) {

	
	//cv::namedWindow("image", 1);
	//cv::imshow("image", image);                   // Show our image inside it.	
	//cv::waitKey(0);
}*/