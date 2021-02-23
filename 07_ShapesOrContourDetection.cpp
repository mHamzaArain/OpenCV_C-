#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

///////////////  Color Detection  //////////////////////

void getContours(Mat imgDil, Mat img) {
	/** @berief find & get contour of standard objects and draw contour,
	outbound rectangle, name of object

	@param imgDil: Dilated image as source
	@param img: Destination(org) image on which we draw all.
	*/

	// Find Contour 
	// Contour ->List contain lists, in each inside list have points(i.e; { {Point(x,y), P(x,y)},.... })
	vector<vector<Point>> contours;
	// Hierarchy -> builtin memeber datatype of OpenCV2, Vec4i = Vector of 4 integers spaces
	vector<Vec4i> hierarchy;

	// Finding contour process
	findContours(imgDil,
		contours,				// contour
		hierarchy,				// hierarchy 
		RETR_EXTERNAL,			// alogorithm to solve problem
		CHAIN_APPROX_SIMPLE);	// Chain approx. algo

	//// Draw Contours
	//drawContours(img,				// Dst image on which we want to draw contour
	//	contours,					// contours matrix
	//	-1,							// no. of contours -> -1 means all contorus have to be drawn
	//	Scalar(255, 0, 255), 2);	// Color -> Purple

	// ContourPoly  
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	float peri;

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

			// Draw Contours on outline of filterd objects
			drawContours(img,				// Dst image on which we want to draw contour
				conPoly,					// contours matrix
				i,							// no. of contours -> -1 means all contorus have to be drawn
				Scalar(255, 0, 255), 2);	// Color -> Purple

			// Print filtered objects
			cout << conPoly[i].size() << endl;

			//// 2. Bounding Rectangle around filtered objects

			// Taking orginal filtered object coordinates
			boundRect[i] = boundingRect(conPoly[i]);

			// Draw rectangle outbound of filtered objects
			rectangle(img,					// Dst image on which we want to draw out bound rectangle
				boundRect[i].tl(),			// Top left of contour matrix
				boundRect[i].br(),			// Bottom right of contour matrix
				Scalar(0, 255, 0), 5);		// Color -> Green

			//// 3. Inserting text
			// Typecasting float into whole number
			int objCor = (int)conPoly[i].size();

			// Rectangle obj
			if (objCor == 3) 
				{ objectType = "Tri"; }
			
			// Square/Rectangle obj
			else if (objCor == 4){
				/* ideal case: width/ height = 1 means it's square
				reality it may vary threrefore we set range.
				*/
				float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
				cout << aspRatio << endl;
				if (aspRatio > 0.95 && aspRatio < 1.05)
					{ objectType = "Square"; }
				else 
					{ objectType = "Rect"; }
			}
			else if (objCor > 4) 
				{ objectType = "Circle"; }


			// Putting text on top of out bound of rectangle
			putText(img,									// Dst image on which we want to put text
				objectType,									// 3=tri, 4=rect, >4 = circle
				{											// Position where to put text
					boundRect[i].x,								// x-axis
					boundRect[i].y - 5							// y-axis & less 5px
				},		
				FONT_HERSHEY_PLAIN,							// Font style
				1,											// Font size 1px
				Scalar(0, 69, 255), 2);						// Color -> Near red
		}
	}
}


void main() {
	string path = "Resources/shapes.png";
	Mat img = imread(path);
	Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;

	// gray
	cvtColor(img,         // Source
		imgGray,		  // Destination 
		COLOR_BGR2GRAY);  // Param to convert color into grey image			

	// Blur
	GaussianBlur(imgGray,   // Source
		imgBlur,			// Destination
		Size(3, 3),			// Kernal size: Higher kernel size makes image more blurer, always choose odd values as best practice
		3, 0);				// Sigma x ,sigma y -> Sigma high values make image more cartoonish

	// edge detector -> Good practice to make blur before canny image
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

	getContours(imgDil,		// Src images
		img);				// image to draw contour

	// Display images
	imshow("Image", img);
	/*imshow("Image Gray", imgGray);
	imshow("Image Blur", imgBlur);
	imshow("Image Canny", imgCanny);
	imshow("Image Dil", imgDil);
*/
	// Run till infinity
	waitKey(0);
}