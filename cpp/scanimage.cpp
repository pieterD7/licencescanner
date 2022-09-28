#include "./scanimage.h"

Mat init_scanimage(vector<Rect*> boxes) {
	int maxWidth = 0, maxHeight = 0;

	for (int n = 0; n < boxes.size(); n++) {
		maxWidth += boxes[n]->width + PAD;
		if (boxes[n]->height > maxHeight) {
			maxHeight = boxes[n]->height;
		}
	}

	return Mat(Size(maxWidth, maxHeight), CV_8UC1, Scalar(0, 0, 0, 255));
}

vector<Rect*> boxes_to_boxes(vector<Rect *> boxes, int angle) {
	vector<Rect*> ret;

	int x = 0;
	for (int n = 0; n < boxes.size(); n++) {
		Rect* r = new Rect();
		r->x = x;
		r->y = 0;
		r->width = boxes[n]->width + PAD;
		r->height = boxes[n]->height;
		ret.push_back(r);
		x += boxes[n]->width + PAD;
	}
	return ret;
}

Mat combine_boxes(Mat mGray, vector<Rect*> boxes, vector<vector<Point>> contour, int angle) {
	int width = 0;

	Mat ret = init_scanimage(boxes);

	for (int n = 0; n < boxes.size(); n++) {
		Rect s = *boxes[n];
		Rect r = Rect(width, 0, boxes[n]->width + PAD, boxes[n]->height);

		s.x -= PAD / 2;
		s.width += PAD;

		Mat src = mGray(s);
		src.copyTo(ret(r));
		width += boxes[n]->width + PAD;
	}

	return ret;
}

Mat make_scanimage(Mat mGray, vector<Rect*> boxes, vector<vector<Point>> contours, int angle) {

	return combine_boxes(mGray, boxes, contours, angle);
}