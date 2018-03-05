#include <opencv2/opencv.hpp>
#include <iostream>
#include "global.h"
#include "population.h"

void write_image_to_file(int file_num, cv::Mat img) {
	std::string filename = WRITE_PATH + "seg" + std::to_string(file_num) + ".png";
	cv::imwrite(filename, img);
}

void write_images_to_file(Population &p) {
	// get pareto rank
	/*
	for(int i = 0; i < N_PARETO; i++){
		p.draw_black_wite_contour(i); // send in index
		write_image_to_file(i,img);
	}
	*/

}