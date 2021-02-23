#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/////////////////// Images //////////////////////

//void main() {
//	std::string path = "Resources/Test.png";
//
//	// cv::Mat -> Matrix class to handle images
//	// cv.imread(String) -> To read image
//	cv::Mat img = cv::imread(path);
//
//	// cv.imshow(<ImageType>, <Img_binary>)
//	imshow("Image", img);
//	
//	// Not to close if parameter is 0 to remain in infinity
//	cv::waitKey(0);  
//}

///////////////// Video //////////////////////

//void main() {
//	std::string path = "Resources/test_video.mp4";
//	
//	// To read whole video in binary
//	cv::VideoCapture cap(path);
//	cv::Mat img;
//	
//	// Run untill no frame left
//	while (true) {
//		// Read, display, halt for 20 milli seconds
//		cap.read(img); 
//		cv::imshow("Image", img);
//		cv::waitKey(20);
//	}
//}

/////////////// Webcam //////////////////////

void main() {

	VideoCapture cap(0);
	Mat img;

	while (true) {

		cap.read(img);
		imshow("Image", img);
		waitKey(1);
	}
}





