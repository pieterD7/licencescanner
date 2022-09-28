#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/core/utils/logger.hpp>

using namespace cv;
using namespace std;
using namespace cv::dnn;

typedef struct MatrixElement {
	int b;	
	int bb;
	MatrixElement(int b, int bb)
	{
		this->b = b;
		this->bb = bb;
	};

} MatrixElement;


typedef struct RecognitionResult {
	string first;
	string second;
	string third;

	// Only when second part has three positions?
	string fourth;
	RecognitionResult() {
	}
	string toString() {
		if (this->first.size() > 0 and this->second.size() > 0 and this->third.size() > 0)
			return this->first + "-" + this->second + this->fourth + "-" + this->third;
		else
			return this->first + this->second;
	}
	string toCode() {
		return this->first + this->second + this->fourth + this->third;
	}
} RecognitionResult;


typedef struct Characteristic {
	int rotation_angle;
	Rect* rect;

	Characteristic(Rect* rect, int rotaion_angle) {
		this->rect = rect;
		this->rotation_angle = rotaion_angle;
	}
} Characteristic;