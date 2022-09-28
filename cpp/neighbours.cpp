#include "./neighbours.h"
#include "./util.h"

double distance(Point p1, Point p2)
{
	return (double)abs(sqrt(pow((double)p2.x - p1.x, 2) + pow((double)p2.y - p1.y, 2)));
}

bool angle_within_range(double ang) {
	ang = abs(ang);
	return ((ang > 0 - AB_DEGREES and ang < 0 + AB_DEGREES)
		or (ang > 180 - AB_DEGREES and ang < 180 + AB_DEGREES));
}

bool same_height( double h, double hh) {
	if(h < hh + hh * AB
		and h > hh - hh * AB)
		return true;
	else
		return false;
}

bool same_height(Rect* b, Rect* bb){ 
	double h = max(b->height, bb->height);
	double hh = min(b->height, bb->height);
	int height = b->height;
	int bbH = bb->height;

	if (hh < h + h * AB
		and h > hh - hh * AB
		and height < bbH + bbH * AB
		and height > bbH - bbH * AB)
		return true;
	else
		return false;
}

int mid(Rect* rect) {
	return rect->y + rect->height / 2;
}

/*
	The characteristic isn't vertically aligned in the middle, but the base of it is the middle
*/
int midCh(Rect* rect) {
	return rect->y + rect->height;
}


int angle() {
	return 0;
}

/*
	Calculate the distance from one block to the next
*/
vector<MatrixElement*> make_matrix(vector<Rect*> boxes, int width, int height)
{
	vector<MatrixElement*> m;

	for (int b = 0; b < boxes.size(); b++) {
		
		double minDist = 9999999;
		int winner = -1;

		for (int bb = b + 1; bb < boxes.size(); bb++) {

            // Is character maybe cut off by border?
            if(boxes[b]->x < 2 || boxes[b]->x + boxes[b]->width > width - 2
            || boxes[bb]->x < 2 || boxes[bb]->x + boxes[bb]->width > width - 2)
                continue;
            if(boxes[b]->y + boxes[b]->height > height - 2 || boxes[b]->y < 2
            || boxes[bb]->y + boxes[b]->height > height - 2 || boxes[bb]->y <2)
                continue;

			// Is to the right?
			if (boxes[bb]->x > boxes[b]->x) {
				double dist = distance(Point(boxes[b]->x, boxes[b]->y), Point(boxes[bb]->x, boxes[bb]->y));
				double ang = angle(Point(boxes[b]->x, boxes[b]->y), Point(boxes[bb]->x, boxes[bb]->y));
				if (same_height(boxes[b], boxes[bb])
					and angle_within_range(ang) ) {
					if (dist < minDist
						and dist < MAX_DIST_H * boxes[b]->height) {
						minDist = dist;
						winner = bb;
					}
				}
			}
		}

		if( winner > -1){
			if (m.size() < MAX_SIZE_MATRIX) {
				MatrixElement* mel = new MatrixElement(b, winner);
				m.insert(m.end(), mel);
			}
		}
	}

	return m;
}
