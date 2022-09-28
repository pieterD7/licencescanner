#include "./util.h"

bool cmp_left_to_right(const Rect* a, const Rect* b)
{
	return a->x < b->x;
}

bool cmp_right_to_left(const Rect* a, const Rect* b)
{
	return a->x > b->x;
}

bool cmp_bottom_to_top(const Rect* a, const Rect* b)
{
	return a->y > b->y;
}

bool cmp_top_to_bottom(const Rect* a, const Rect* b)
{
	return a->y < b->y;
}

bool near(double x1, double x2, double ab) {
	return x1 > x2 - ab and x1 < x2 + ab;
}

double angle(Point p1, Point p2)
{
	double angle = atan2((double)p1.y - p2.y, (double)p1.x - p2.x);
	return angle * 180 / M_PI;
}

Rect bounding_rect(vector<Rect*> rects)
{
	int xmax = 0;
	int ymax = 0;
	int xmin = 99999;
	int ymin = 99999;

	for (int i = 0; i < rects.size(); i++)
	{
		if (rects[i]->x < xmin)
			xmin = rects[i]->x;
		if (rects[i]->y < ymin)
			ymin = rects[i]->y;
		if (rects[i]->x + rects[i]->width > xmax)
			xmax = rects[i]->x + rects[i]->width;
		if (rects[i]->y + rects[i]->height > ymax)
			ymax = rects[i]->y + rects[i]->height;
	}
	return Rect(xmin, ymin, xmax - xmin, ymax - ymin);
}

int avg_width(vector<Rect*> rects) {
	int sum = 0;
	for (int n = 0; n < rects.size(); n++) {
		sum += rects[n]->width;
	}
	return sum / int(rects.size());
}

vector<Rect*> combine(vector<Rect*> v1, vector<Rect*> v2) {
	vector<Rect*> combined;
	combined.reserve(v1.size() + v2.size());
	combined.insert(combined.end(), v1.begin(), v1.end());
	combined.insert(combined.end(), v2.begin(), v2.end());
	return combined;
}

bool rectangles_overlap(Rect* rect1, Rect* rect2) {
	return (*rect1 & *rect2).area() > 0;
}

Point midpoint(Point pnt1, Point pnt2) {
	return (pnt1 + pnt2) * 0.5;
}

Point center(Rect rect) {
	return midpoint(rect.tl(), rect.br());
}

Rect overlap(Rect rect1, Rect rect2) {
	return (rect1 & rect2);
}

Rect add(Rect rect1, Rect rect2) {
	return (rect1 | rect2);
}

bool is_inside(Rect rect, Mat image) {
	return overlap(rect, Rect(0, 0, image.cols, image.rows)) == rect;
}