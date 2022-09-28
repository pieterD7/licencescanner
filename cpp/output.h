#pragma once
#include "./licence.h"

vector<Rect *> get_type(Mat mGray, int distance);

vector<Rect> get_blobs(Mat mGray, int diistance);
