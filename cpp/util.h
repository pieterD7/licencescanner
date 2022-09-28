#pragma once

#include "./licence.h"

bool cmp_left_to_right(const Rect* a, const Rect* b);

bool cmp_right_to_left(const Rect* a, const Rect* b);

bool cmp_bottom_to_top(const Rect* a, const Rect* b);

bool cmp_top_to_bottom(const Rect* a, const Rect* b);

bool near(double x1, double x2, double ab);

double angle(Point p1, Point p2);

Rect bounding_rect(vector<Rect*> rects);

int avg_width(vector<Rect*> rects);

vector<Rect*> combine(vector<Rect*> v1, vector<Rect*> v2);

bool rectangles_overlap(Rect* rect1, Rect* rect2);

Point midpoint(Point pnt1, Point pnt2);

Point center(Rect rect);

bool is_inside(Rect rect, Mat image);

Rect add(Rect rect1, Rect rect2);

Rect overlap(Rect rect1, Rect rect2);