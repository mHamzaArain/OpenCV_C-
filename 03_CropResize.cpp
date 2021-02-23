#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


///////////////  Resize and Crop //////////////////////

void main() {
	string path = "Resources/test.png";
	Mat img = imread(path);
	Mat imgResize, imgCrop;

	//cout << img.size() << endl;		// [768 x 559]
	resize(img,			// Src
		imgResize,		// Dst
		Size(),			// Size of image
		0.5,			// fx scale factor along the horizontal axis, 0= 0% compression & 1= 100% compression 
		0.5);			// fy scale factor along the vertical axis, 0= 0% compression & 1= 100% compression

	// Image container
	Rect roi(200,		// x-ordinate
		100,			// y-ordinate
		300,			// width
		300);			// height
	
	imgCrop = img(roi);  // Putting image inside container
	
	// Showing images
	imshow("Image", img);
	imshow("Image Resize", imgResize);
	imshow("Image Crop", imgCrop);

	// Run till infinity
	waitKey(0);
}
