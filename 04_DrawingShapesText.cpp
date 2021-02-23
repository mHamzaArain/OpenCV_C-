#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//////////////  Draw Shapes and Text //////////////////////

void main() {
	// Blank(White) Image 
	Mat img(512, 512,					// Length, width
		CV_8UC3,						// 8U -> 8-bits & UNsigned means that each pixel has value 0-255
										// C3 -> Channel 3 means each pixel has three channels(i.e; RGB)
		Scalar(255, 255, 255));			// Define color -> Here each pixel is of white

	// Circle -> Drow circle on image
	circle(img,							// white image we created
		Point(256, 256),				// Point of origin(centre)
		155,							// size of circle in terms of pixel
		Scalar(0, 69, 255),				// Color -> Orange
		FILLED);						// Fill color in circle

	// Rectangle -> Draw rectangle on circle
	rectangle(img,						// white image we created
		Point(130, 226),				// bottom left & right 
		Point(382, 286),				// upper left & right
		Scalar(255, 255, 255),			// Color -> White
		FILLED);						// Fill color in circle
	
	// Line -> Draw line on circle beneath rectangle
	line(img,							// white image we created
		Point(130, 296),				// x1, y1
		Point(382, 296),				// x2, y2
		Scalar(255, 255, 255),			// Color -> White
		2);								// Width size is 2
	
	// Text -> Inserting text
	putText(img,						// white image we created
		"Murtaza's Workshop",			// text to be displayed
		Point(137, 262),				// xy-coordinate
		FONT_HERSHEY_DUPLEX,			// font style
		0.75,							// font size(Scale)
		Scalar(0, 69, 255),				// Color -> Orange
		2);								// Width size is 2

	// Display image
	imshow("Image", img);

	// Run till infinity
	waitKey(0);
}