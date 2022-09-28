#pragma once

#include "./licence.h"

Mat analyse(Mat mGray, Mat mColor, int preview );

string analyse_mat(Mat mGray);

vector<Rect*> get_type();

vector<vector<Rect*>> get_bxs();

vector<MatrixElement*> get_dmatrix();
