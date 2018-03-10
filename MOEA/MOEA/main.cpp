#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "population.h"
#include <string>
#include <iostream>
#include <ctime>
#include "var_operators.h"
#include "global.h"
#include "file.h"

int main() {
	srand(time(0));

	Population p;
	p.initialize_population();

	int generation = 0;
	time_t seconds = time(NULL);
	
	while (generation < N_GENERATIONS) {
		p.MOEA_next_generation();
		std::cout << "generation nr " << generation << std::endl;
		generation++;
	}
	std::cout << "finished " << std::endl;
	std::cout << seconds - time(NULL) << std::endl;
	seconds = time(NULL);
	p.draw_pareto_front();

	/*std::vector<int> survivors;
	survivors.push_back(0);
	survivors.push_back(1);
	survivors.push_back(4);
	survivors.push_back(5);
	survivors.push_back(7);
	survivors.push_back(3);
	std::vector<int> non_survivors;
	std::vector<int> survivors_offspring;

	for (int i = 0; i < 8; i++) {
		if (find(survivors.begin(), survivors.end(), i) == survivors.end() && i < 6) {
			if (i < 6) {
				non_survivors.push_back(i); // don't care if it is higher than N_IND
			}
		}
		else{
			if (i >= 6) {
				survivors_offspring.push_back(i);
			}
		}
	}
	
	if (survivors_offspring.size() != non_survivors.size()) std::cout << "different size" << survivors_offspring.size() << " " << non_survivors.size()<<  std::endl;
	//std::cin.get();
	for (int i = 0; i < non_survivors.size(); i++) {
		std::cout << " switch " << non_survivors[i] << " " << survivors_offspring[i] << std::endl;
		//population[non_survivors[i]] = population[survivors_offspring[i]]; // again; should it be deep copy? Look for pointer error in destructor.
	}/*

	
	/*Population p;
	p.initialize_population();
	p.draw_segments_contour(0, 0);
	cv::Mat img = p.draw_segments_black_contour(0);
	//(window_name, 1);
	//cv::imshow(window_name, img);
	writ_image_to_file(0, img);
	cv::waitKey(0);
	//writ_image_to_file(0, cv::Mat img)*/
	

	//std::cout << p.get_im_h() *p.get_im_w() << std::endl;

	/*p.initialize_population_test();
	std::stack<pos> branch_points;
	pos pos_i(0,0);
	while(pos_i.x != static_cast<unsigned short>(-1)) {
		std::cout << pos_i.x << ", " << pos_i.y << std::endl;
		pos_i = p.traverse_ST(0, pos_i, branch_points);
	}
	std::cout << "---" << std::endl;
	pos_i = pos(2, 1);
	while (pos_i.x != static_cast<unsigned short>(-1)) {
		std::cout << pos_i.x << ", " << pos_i.y << std::endl;
		pos_i = p.traverse_ST(0, pos_i, branch_points);
	}*/

	system("pause");
	return 1;
}

/*
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <iostream>
using namespace cv;
using namespace std;
static void help()
{
	cout
		<< "\nThis program illustrates the use of findContours and drawContours\n"
		<< "The original image is put up along with the image of drawn contours\n"
		<< "Usage:\n"
		<< "./contours2\n"
		<< "\nA trackbar is put up which controls the contour level from -3 to 3\n"
		<< endl;
}
const int w = 500;
int levels = 3;
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
static void on_trackbar(int, void*)
{
	Mat cnt_img = Mat::zeros(w, w, CV_8UC3);
	int _levels = levels - 3;
	drawContours(cnt_img, contours, _levels <= 0 ? 3 : -1, Scalar(128, 255, 255),
		3, LINE_AA, hierarchy, std::abs(_levels));
	imshow("contours", cnt_img);
}
int main(int argc, char** argv)
{
	cv::CommandLineParser parser(argc, argv, "{help h||}");
	if (parser.has("help"))
	{
		help();
		return 0;
	}
	Mat img = Mat::zeros(w, w, CV_8UC1);
	//Draw 6 faces
	for (int i = 0; i < 6; i++)
	{
		int dx = (i % 2) * 250 - 30;
		int dy = (i / 2) * 150;
		const Scalar white = Scalar(255);
		const Scalar black = Scalar(0);
		if (i == 0)
		{
			for (int j = 0; j <= 10; j++)
			{
				double angle = (j + 5)*CV_PI / 21;
				line(img, Point(cvRound(dx + 100 + j * 10 - 80 * cos(angle)),
					cvRound(dy + 100 - 90 * sin(angle))),
					Point(cvRound(dx + 100 + j * 10 - 30 * cos(angle)),
						cvRound(dy + 100 - 30 * sin(angle))), white, 1, 8, 0);
			}
		}
		ellipse(img, Point(dx + 150, dy + 100), Size(100, 70), 0, 0, 360, white, -1, 8, 0);
		ellipse(img, Point(dx + 115, dy + 70), Size(30, 20), 0, 0, 360, black, -1, 8, 0);
		ellipse(img, Point(dx + 185, dy + 70), Size(30, 20), 0, 0, 360, black, -1, 8, 0);
		ellipse(img, Point(dx + 115, dy + 70), Size(15, 15), 0, 0, 360, white, -1, 8, 0);
		ellipse(img, Point(dx + 185, dy + 70), Size(15, 15), 0, 0, 360, white, -1, 8, 0);
		ellipse(img, Point(dx + 115, dy + 70), Size(5, 5), 0, 0, 360, black, -1, 8, 0);
		ellipse(img, Point(dx + 185, dy + 70), Size(5, 5), 0, 0, 360, black, -1, 8, 0);
		ellipse(img, Point(dx + 150, dy + 100), Size(10, 5), 0, 0, 360, black, -1, 8, 0);
		ellipse(img, Point(dx + 150, dy + 150), Size(40, 10), 0, 0, 360, black, -1, 8, 0);
		ellipse(img, Point(dx + 27, dy + 100), Size(20, 35), 0, 0, 360, white, -1, 8, 0);
		ellipse(img, Point(dx + 273, dy + 100), Size(20, 35), 0, 0, 360, white, -1, 8, 0);
	}
	//show the faces
	namedWindow("image", 1);
	imshow("image", img);
	//Extract the contours so that
	vector<vector<Point> > contours0;
	findContours(img, contours0, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	contours.resize(contours0.size());
	for (size_t k = 0; k < contours0.size(); k++)
		approxPolyDP(Mat(contours0[k]), contours[k], 3, true);
	namedWindow("contours", 1);
	createTrackbar("levels+3", "contours", &levels, 7, on_trackbar);
	on_trackbar(0, 0);
	waitKey();
	return 0;
}*/