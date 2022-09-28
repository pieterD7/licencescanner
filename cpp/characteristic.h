#pragma once
#include "./licence.h"

vector<Characteristic> characteristic(vector<vector<Point>>& contours, vector<Rect>& blobs);

vector<Rect*> filter_ch(vector<Characteristic> ch, vector<MatrixElement*> matrix, vector<Rect*> boxes, Point topLeft);

int get_rotation_angle();