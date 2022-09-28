#pragma once

#include "./licence.h"

void detect_blobs(Mat mGray, vector< vector<Point> >& contours, vector<Rect>& mser_bbox);

void detect_blobs_ch(Mat mGray, vector< vector<Point> >& contours, vector<Rect>& mser_bbox);

vector<Rect*> unique_blobs(vector<Rect*> blobs);

vector<Rect*> unique_blobs(vector<Characteristic> ch);

vector<Rect* > filter_type(vector<Rect>& blobs);
