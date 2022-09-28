#pragma once

#include "./licence.h"
#include "./constants.h"

vector<vector<Rect*>> get_candidates(vector<Rect*> boxes, vector<MatrixElement*> dmatrix);

void evaluate_ch(vector<Rect*> ch, vector<Rect*> found1, bool hasTwoLines, int sizeFirstLine);

bool get_has_two_lines();

int get_size_first_line();

int get_first();

int get_second();

int get_third();

int get_fourth();