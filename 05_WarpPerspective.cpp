#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

///////////////  Warp Images  //////////////////////

void main() {
	string path = "Resources/cards.jpg";
	Mat img = imread(path);
	Mat kingMatrix, kingImgWarp,
		queenNatrix, queenImgWarp;
	float w = 250, h = 350;

	//// King card
	// Original vertices
	Point2f kingSrc[4] = { {529,142},		// upper left
							{771,190},		// upper right
							{405,395},		// bottom left
							{674,457} };	// bottom right
	// Transformable vertices
	Point2f kingDst[4] = { {0.0f,0.0f},		// upper left
							{w,0.0f},		// upper right
							{0.0f,h},		// bottom left
							{w,h} };	    // bottom right

	// Matching Src & Dst coordinates
	kingMatrix = getPerspectiveTransform(kingSrc, kingDst); 
	warpPerspective(img,		// Src
		kingImgWarp,			// Dst
		kingMatrix,				// Image interpolation
		Point(w, h));			// Size image

	// Draw points on chosen coordinates
	for (int i = 0; i < 4; i++)
	{
		// Circle -> Drow circle on image
		circle(img,							// white image we created
			kingSrc[i],						// Point of origin(centre)
			10,							// size of circle in terms of pixel
			Scalar(0, 69, 255),				// Color -> Orange
			FILLED);						// Fill color in circle
	};


	//// Queen card
	// Original vertices
	Point2f queenSrc[4] = { {67, 324},		// upper left
						    {332, 279},		// upper right
						    {95, 634},      // bottom left
						    {403, 569} };   // bottom right

	// Transformable vertices
	Point2f queenDst[4] = { {0.0f,0.0f},    // upper left
							{w,0.0f},		// upper right
							{0.0f,h},		// bottom left
							{w,h} };	    // bottom right

	// Matching Src & Dst coordinates
	queenNatrix = getPerspectiveTransform(queenSrc, queenDst);
	warpPerspective(img,	// Src
		queenImgWarp,		// Dst
		queenNatrix,		// image interpolation
		Point(w, h));		// size of image

	// Draw points on chosen coordinates
	for (int i = 0; i < 4; i++)
	{
		// Circle -> Drow circle on image
		circle(img,							// white image we created
			queenSrc[i],					// Point of origin(centre)
			10,								// size of circle in terms of pixel
			Scalar(0, 69, 255),				// Color -> Orange
			FILLED);						// Fill color in circle
	}

	// Displaying image
	imshow("Image", img);
	imshow("King Image Warp", kingImgWarp);
	imshow("Queen Image Warp", queenImgWarp);
	
	// Rum till infinity
	// Rum till infinity
	waitKey(0);

}