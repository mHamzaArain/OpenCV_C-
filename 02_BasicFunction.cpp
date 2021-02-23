#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

///////////////  Basic Functions  //////////////////////

void main() {
	string path = "Resources/test.png";
	Mat img = imread(path);
	Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;

	// grey
	cvtColor(img,         // Source
		imgGray,		  // Destination 
		COLOR_BGR2GRAY);  // Param to convert color into grey image			
	
	// Blur
	GaussianBlur(imgGray,   // Source
		imgBlur,			// Destination
		Size(7, 7),			// Kernal size: Higher kernel size makes image more blurer, always choose odd values as best practice
		5, 0);				// Sigma x ,sigma y -> Sigma high values make image more cartoonish
	
	//  edge detector -> Good practice to make blur before canny image
	Canny(imgBlur,			// Source
		imgCanny,			// Destination
		25, 75);			// thresould 1, threshould 2 -> hysteresis procedure


	// Kernel for dilation/erode
	Mat kernel = getStructuringElement(MORPH_RECT,
										Size(3, 3));	// Size directly proportional
	
	// Increase thickness
	dilate(imgCanny,		// Source
		imgDil,				// Destination
		kernel);			// Kernal as made above

	// Decrease thickness
	erode(imgDil,			// Source
		imgErode,			// Destination
		kernel);			// Kernal as made above

	// Showing images
	imshow("Image", img);
	imshow("Image Gray", imgGray);
	imshow("Image Blur", imgBlur);
	imshow("Image Canny", imgCanny);
	imshow("Image Dilation", imgDil);
	imshow("Image Erode", imgErode);

	// Run till infinity
	waitKey(0);
}
