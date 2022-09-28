#pragma once
#include "./licence.h"

RecognitionResult recognize(Mat image, vector<Rect*> boxes, int first, int second, int third, int fourth);

void init_recognition(string dictionary, string model);

void init_recognition();