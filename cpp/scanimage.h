#pragma once
#include "./licence.h"

/*
	Licences on two lines need to become one line for recognition
*/

#define PAD 8

Mat make_scanimage(Mat mGray, vector<Rect*> boxes, vector<vector<Point>> contour, int angle);

vector<Rect*> boxes_to_boxes(vector<Rect*> boxes, int angle);