#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


///////////////// Virtual Painter //////////////////////

Mat img;
VideoCapture cap(0);
vector<vector<int>> newPoints;  // to store all points

/////////////////////  COLOR VALUES ////////////////////////////////


// hmin, smin, vmin hmax, smax, vmax
vector<vector<int>> myColors{
	{38,72,132,141,255,248},	// blue
	{151,151,77,179,255,255}		// Red
};

// It follows BGR > Blue, Green, Red to draw on screen
vector<Scalar> myColorValues{
	{255,0,0},		// Blue
	{0,0,255}		// Red
};

////////////////////////////////////////////////////////////////////

Point getContours(Mat image) {
	/** @berief find & get contour of cap of marker & return Point of top middle of marker.
	@param imageg: Destination(org) image on which we draw all.
	*/

	// Find Contour 
	// Contour ->List contain lists, in each inside list have points(i.e; { {Point(x,y), P(x,y)},.... })
	vector<vector<Point>> contours;
	// Hierarchy -> builtin memeber datatype of OpenCV2, Vec4i = Vector of 4 integers spaces
	vector<Vec4i> hierarchy;

	// Finding contour process
	findContours(image,			// Src image
		contours,				// contour
		hierarchy,				// hierarchy 
		RETR_EXTERNAL,			// alogorithm to solve problem
		CHAIN_APPROX_SIMPLE);	// Chain approx. algo

	//// Draw infinite Contours
	//drawContours(img,				// Dst image on which we want to draw contour
	//	contours,					// contours matrix
	//	-1,							// no. of contours -> -1 means all contorus have to be drawn
	//	Scalar(255, 0, 255), 2);	// Color -> Purple

	// ContourPoly  
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	float peri;

	Point myPoint(0, 0);

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;		// Area of contour point without filtration
		string objectType;

		// area must be greater than 1000px otherwise it will neglect smaller values 
		if (area > 1000)
		{
			//// 1. Bounding box(outline) around shapes
			// perimeter of an arc(part of a circle)
			// Arc length is the distance between two points along a section of a curve. 
			peri = arcLength(contours[i],		// identified contour point 
				true);			    // object is closed or not

			approxPolyDP(contours[i],			// identified contour point
				conPoly[i],						// send contour point
				0.02 * peri,					// formula to precise 
				true);							// object is closed or not

			// Print filtered objects
			cout << conPoly[i].size() << endl;

			//// 2. Bounding Rectangle around filtered objects

			// Taking orginal filtered object coordinates
			boundRect[i] = boundingRect(conPoly[i]);

			//// 3. X & Y axis of middle point of marker cap to draw exactly from middle point

			// x-> Middle point of x-axis(To draw exactly from middle)
			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			// y-> y-axis remain same
			myPoint.y = boundRect[i].y;

			//// Draw Contours on outline of filterd objects
			//drawContours(img,				// Dst image on which we want to draw contour
			//	conPoly,					// contours matrix
			//	i,							// no. of contours -> -1 means all contorus have to be drawn
			//	Scalar(255, 0, 255), 2);	// Color -> Purple

			//// Draw rectangle outbound of filtered objects
			//rectangle(img,					// Dst image on which we want to draw out bound rectangle
			//	boundRect[i].tl(),			// Top left of contour matrix
			//	boundRect[i].br(),			// Bottom right of contour matrix
			//	Scalar(0, 255, 0), 5);		// Color -> Green
		}
	}
	return myPoint;
}

vector<vector<int>> findColor(Mat img) {
	/** @berief Find the color of cap of marker
	@param img: Src image from which we find color of marker.
	*/

	/**	How HSV works??
	----------------
	HSV is a cylindrical color model that remaps the RGB primary colors into
	dimensions that are easier for humans to understand. ... Hue specifies
	the angle of the color on the RGB color circle. A 0° hue results in red,
	120° results in green, and 240° results in blue. Saturation controls the
	amount of color used.
	*/

	Mat imgHSV;

	// HSV image -> To detect ranges of color
	cvtColor(img,				// Src
		imgHSV,					// Dst
		COLOR_BGR2HSV);			//  HSV (hue, saturation, value

	for (int i = 0; i < myColors.size(); i++)
	{
		Scalar lower(
			myColors[i][0],
			myColors[i][1],
			myColors[i][2]);

		Scalar upper(
			myColors[i][3],
			myColors[i][4],
			myColors[i][5]);

		Mat mask;

		inRange(imgHSV, lower, upper, mask);
		//imshow(to_string(i), mask);

		Point myPoint = getContours(mask);

		if (myPoint.x != 0) {
			newPoints.push_back({ myPoint.x,myPoint.y,i });
		}
	}
	return newPoints;
}

void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues) {
	/** @berief Draw circle in caputured coordinates where top-middle of marker placed.
	@param newPoints: New captured point on which circle to be drawn.
	@param myColorValues: list inside list having scalar value (BGR values) respected
	to be drawn in circle.
	*/

	for (int i = 0; i < newPoints.size(); i++) {
		// Circle -> Drow circle on image
		circle(img,								// Src image
			Point(								// Coordinates where middle of tip of marker placed
				newPoints[i][0],						// X-axis of top-middle of marker
				newPoints[i][1]							// Y-axis of top of pointer
			),
			10,									// size of circle in terms of pixel
			myColorValues[newPoints[i][2]],		// Color
			FILLED);							// Fill color in circle
	}
}


void main() {
	while (true) {
		// Read captured frame continuously
		cap.read(img);

		// Find coordinates on captured frame consecutively
		newPoints = findColor(img);

		// Draw color dots on captured points coontinuously
		drawOnCanvas(newPoints, myColorValues);

		// Display each frame in sequency
		imshow("Image", img);

		// Run till infinity
		waitKey(1);
	}
}
