#include "./characteristic.h"
#include "./util.h"

#define CHARACTERISTIC_MIN_RATIO 0.4

#define CHARACTERISTIC_MAX_RATIO 0.65

int rotation_angle;

int get_rotation_angle() {
	return rotation_angle;
}

bool is_ch(double width, double height) {
	return (height < 30 and 
		width > 5 and
		height < width and 
		height / width > CHARACTERISTIC_MIN_RATIO and
		height / width < CHARACTERISTIC_MAX_RATIO);
}

bool outside(Rect* rect, vector<MatrixElement*> matrix, vector<Rect*> boxes) {
	for (int m = 0; m < matrix.size(); m++) {
		if (rectangles_overlap(rect, boxes[matrix[m]->b]))
			return false;
	}
	return true;
}

vector<Characteristic> characteristic(vector<vector<Point>>& contours, vector<Rect>& blobs) {
	vector<Characteristic> t;
	for (int i = 0; i < contours.size(); i++) {
		RotatedRect rect = minAreaRect(contours[i]);
		if (rect.size.width < rect.size.height and
			rect.angle < 90 and
			blobs[i].width > blobs[i].height) {
			if (is_ch(rect.size.height, rect.size.width)) {
				t.insert(t.end(), Characteristic(&blobs[i], (int)rect.angle - 90));
			}
		}
		else if (rect.angle < 90) {
			if (is_ch(rect.size.width, rect.size.height)) {
				t.insert(t.end(), Characteristic(&blobs[i], (int)rect.angle));
			}
		}
		else
			if (is_ch(blobs[i].width, blobs[i].height))
				t.insert(t.end(), Characteristic(&blobs[i], 0));
	}
	return t;
}

vector<Rect*> filter_ch(vector<Characteristic> ch, vector<MatrixElement*> matrix, vector<Rect*> boxes, Point topLeft) {
	vector<Rect*> ret;
	for (int n = 0; n < ch.size(); n++) {

		ch[n].rect->x += topLeft.x;
		ch[n].rect->y += topLeft.y;

		for (int m = 0; m < matrix.size(); m++) {
			int chX = ch[n].rect->x;
			int chY = ch[n].rect->y;
			int mY1 = boxes[matrix[m]->b]->y;
			int mX1 = boxes[matrix[m]->b]->x;
			int mX2 = boxes[matrix[m]->bb]->x;
			int h = boxes[matrix[m]->b]->height;
			if (near(mY1, chY, h * 0.5) and
				near(mX1, chX, h) and
				chX > mX1 and chX < mX2 and
				near(mY1 + h / 2, chY, h / 4)) {

				rotation_angle = ch[n].rotation_angle;
				ret.insert(ret.end(), ch[n].rect);
			}
		}
	}

	// All ch's must be between two boxes
	for (int n = (int)ret.size() - 1; n > -1; n--) {
		if (!outside(ret[n], matrix, boxes))
			ret.erase(ret.begin() + n);
	}

	return ret;
}
