/*
	All characters need to have a coresponding neighbour (same height and near by on same line)
*/

#pragma once
#include "./licence.h"
#include "./constants.h"
#include "./candidates.h"

vector<MatrixElement*> 	make_matrix(vector<Rect*> boxes, int width, int height);