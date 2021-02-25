#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

///////////////  Project 2 - Document Scanner  //////////////////////

Mat imgOriginal, imgGray, imgBlur, imgCanny, imgThre, imgDil, imgErode, imgWarp, imgCrop;
vector<Point> initialPoints, docPoints;
float w = 420, h = 596;

Mat preProcessing(Mat img) {
	// grey
	cvtColor(img,         // Source
		imgGray,		  // Destination 
		COLOR_BGR2GRAY);  // Param to convert color into grey image			

	// Blur
	GaussianBlur(imgGray,   // Source
		imgBlur,			// Destination
		Size(3, 3),			// Kernal size: Higher kernel size makes image more blurer, always choose odd values as best practice
		3, 0);				// Sigma x ,sigma y -> Sigma high values make image more cartoonish

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

	//// Decrease thickness
	//erode(imgDil,			// Source
	//	imgErode,			// Destination
	//	kernel);			// Kernal as made above

	return imgDil;
}

vector<Point> getContours(Mat image) {
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

	vector<Point> biggest;
	int maxArea = 0;
	float peri;

	for (int i = 0; i < contours.size(); i++) {
		int area = contourArea(contours[i]);
		//cout << area << endl;		// Area of contour point without filtration
		string objectType;

		// area must be greater than 1000px otherwise it will neglect smaller values 
		if (area > 1000) {
			//// 1. Bounding box(outline) around shapes
			// perimeter of an arc(part of a circle)
			// Arc length is the distance between two points along a section of a curve. 
			peri = arcLength(contours[i],		// identified contour point 
							true);			    // object is closed or not

			approxPolyDP(contours[i],			// identified contour point
				conPoly[i],						// send contour point
				0.02 * peri,					// formula to precise 
				true);							// object is closed or not

			// Area must be greater than (Maxsize = 0) & has 4 edges
			if (area > maxArea && conPoly[i].size() == 4) {
				//// Draw Contours on outline of filterd objects
				//drawContours(img,				// Dst image on which we want to draw contour
				//	conPoly,					// contours matrix
				//	i,							// no. of contours -> -1 means all contorus have to be drawn
				//	Scalar(255, 0, 255), 2);	// Color -> Purple

				biggest = { conPoly[i][0],conPoly[i][1] ,conPoly[i][2] ,conPoly[i][3] };
				maxArea = area;
			}

			//// Draw Contours on outline of filterd objects
			//drawContours(img,				// Dst image on which we want to draw contour
			//	conPoly,					// contours matrix
			//	-1,							// no. of contours -> -1 means all contorus have to be drawn
			//	Scalar(255, 0, 255), 2);	// Color -> Purple

			//// Draw rectangle outbound of filtered objects
			//rectangle(img,				// Dst image on which we want to draw out bound rectangle
			//	boundRect[i].tl(),			// Top left of contour matrix
			//	boundRect[i].br(),			// Bottom right of contour matrix
			//	Scalar(0, 255, 0), 5);		// Color -> Green
		}
	}
	return biggest;
}

void drawPoints(vector<Point> points, Scalar color) {
	for (int i = 0; i < points.size(); i++) {
		circle(imgOriginal, points[i], 10, color, FILLED);

		// Circle -> Drow circle on image
		circle(imgOriginal,					// white image we created
			points[i],						// Point of origin(centre)
			10,								// size of circle in terms of pixel
			color,							// Color 
			FILLED);						// Fill color in circle

		// Text -> Inserting text
		putText(imgOriginal,			// white image we created
			to_string(i),				// text to be displayed
			points[i],					// xy-coordinate
			FONT_HERSHEY_DUPLEX,		// font style
			4,							// font size(Scale)
			color,						// Color -> Orange
			4);							// Thickness
	}
}

vector<Point> reorder(vector<Point> points)
{
	vector<Point> newPoints;
	vector<int>  sumPoints, subPoints;

	for (int i = 0; i < 4; i++)
	{
		sumPoints.push_back(points[i].x + points[i].y);
		subPoints.push_back(points[i].x - points[i].y);
	}

	newPoints.push_back(points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); // 0
	newPoints.push_back(points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); //1
	newPoints.push_back(points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); //2
	newPoints.push_back(points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); //3

	return newPoints;
}

Mat getWarp(Mat img, vector<Point> points, float w, float h)
{
	Point2f src[4] = { points[0],points[1],points[2],points[3] };
	Point2f dst[4] = { {0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h} };

	Mat matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	return imgWarp;
}

void main() {
	string path = "Resources/paper.jpg";
	imgOriginal = imread(path);
	resize(imgOriginal, imgOriginal, Size(), 0.5, 0.5);

	// Preprpcessing - Step 1 
	imgThre = preProcessing(imgOriginal);

	// Get Contours - Biggest  - Step 2
	initialPoints = getContours(imgThre);
	//drawPoints(initialPoints, Scalar(0, 0, 255));
	docPoints = reorder(initialPoints);
	//drawPoints(docPoints, Scalar(0, 255, 0));

	// Warp - Step 3 
	imgWarp = getWarp(imgOriginal, docPoints, w, h);

	//Crop - Step 4
	int cropVal = 5;
	Rect roi(cropVal, cropVal, w - (2 * cropVal), h - (2 * cropVal));
	imgCrop = imgWarp(roi);

	imshow("Image", imgOriginal);
	imshow("Image Dilation", imgThre);
	imshow("Image Warp", imgWarp);
	imshow("Image Crop", imgCrop);
	waitKey(0);
}