#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main()
{
	VideoCapture cap(0);
	Mat img;
	Mat imgHSV, mask, imgColor;
	int hmin = 0, smin = 0, vmin = 0;
	int hmax = 179, smax = 255, vmax = 255;

	/* How HSV works??
	----------------
	HSV is a cylindrical color model that remaps the RGB primary colors into
	dimensions that are easier for humans to understand. ... Hue specifies
	the angle of the color on the RGB color circle. A 0° hue results in red,
	120° results in green, and 240° results in blue. Saturation controls the
	amount of color used.
	*/

	// Color tracker setting bar
	namedWindow("Trackbars", (640, 200)); // Create Window
	createTrackbar("Hue Min", "Trackbars", &hmin, 179);
	createTrackbar("Hue Max", "Trackbars", &hmax, 179);
	createTrackbar("Sat Min", "Trackbars", &smin, 255);
	createTrackbar("Sat Max", "Trackbars", &smax, 255);
	createTrackbar("Val Min", "Trackbars", &vmin, 255);
	createTrackbar("Val Max", "Trackbars", &vmax, 255);

	// To chage high/low values in runtime to detect different ranges of colors
	while (true) {
		// Read each captured frame 
		cap.read(img);

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

		Scalar lower(hmin, smin, vmin);  // lower range color
		Scalar upper(hmax, smax, vmax);	 // higher range color

		// Recognize color in range(i.e; light/dark red)
		inRange(imgHSV, lower, upper, mask);

		// hmin, smin, vmin, hmax, smax, vmax;
		cout << hmin << "," << smin << "," << vmin << "," << hmax << "," << smax << "," << vmax << endl;

		// Display images
		imshow("Image", img);
		imshow("Image Mask", mask);

		// Run till infinity
		waitKey(1);
	}
}