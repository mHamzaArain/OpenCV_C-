#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

///////////////  Color Detection  //////////////////////

void main() {
	string path = "Resources/shapes.png";
	Mat img = imread(path);
	Mat imgHSV, mask;
	int hmin = 0, smin = 110, vmin = 153;
	int hmax = 19, smax = 240, vmax = 255;


	/*
	How HSV works??
	----------------
	HSV is a cylindrical color model that remaps the RGB primary colors into
	dimensions that are easier for humans to understand. ... Hue specifies 
	the angle of the color on the RGB color circle. A 0° hue results in red,
	120° results in green, and 240° results in blue. Saturation controls the
	amount of color used.
	*/

	// HSV image -> To detect ranges of color
	cvtColor(img,				// Src
		imgHSV,					// Dst
		COLOR_BGR2HSV);			//  HSV (hue, saturation, value

	// Color tracker setting bar
	namedWindow("Trackbars", (640, 200));
	createTrackbar("Hue Min", "Trackbars", &hmin, 179);
	createTrackbar("Hue Max", "Trackbars", &hmax, 179);
	createTrackbar("Sat Min", "Trackbars", &smin, 255);
	createTrackbar("Sat Max", "Trackbars", &smax, 255);
	createTrackbar("Val Min", "Trackbars", &vmin, 255);
	createTrackbar("Val Max", "Trackbars", &vmax, 255);

	// To chage high/low values in runtime to detect different ranges of colors
	while (true) {
		Scalar lower(hmin, smin, vmin);  // lower range color
		Scalar upper(hmax, smax, vmax);	 // higher range color

		// Recognize color in range(i.e; light/dark red)
		inRange(imgHSV, lower, upper, mask);

		// Display images
		imshow("Image", img);
		imshow("Image HSV", imgHSV);
		imshow("Image Mask", mask);
		
		// Run till infinity
		waitKey(1);
	}
}