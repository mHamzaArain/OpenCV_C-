#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;


///////////////  Images  //////////////////////

void main() {
	string path = "Resources/test.jpg";
	Mat img = imread(path);

	// Opened pre-trained model harcascade
	CascadeClassifier faceCascade;
	faceCascade.load("Resources/haarcascade_frontalface_default.xml");

	// Filename if not matched or not present in working folder
	if (faceCascade.empty()) 
		{ cout << "XML file not loaded" << endl; }
	
	//Find face
	vector<Rect> faces;
	faceCascade.detectMultiScale(img,			// Src
		faces,									// Detected face
		1.1,									// Scale factor
		10);									// Minimum neighbour

	for (int i = 0; i < faces.size(); i++) {
		// Rectangle -> Draw rectangle on src image
		rectangle(img,					// Src image
			faces[i].tl(),				// top left
			faces[i].br(),				// bottom right
			Scalar(255, 0, 255),		// Color -> purple
			3);							// Outline 3px
	}

	// Display image
	imshow("Image", img);

	// Run till infinity
	waitKey(0);
}