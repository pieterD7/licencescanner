#include "./output.h"
#include "./detect.h"

/*
	Utility to aid traing a model which dumps blobs as a file in outputDir.
*/
vector<Rect *> get_type(Mat mGray, int distance) {

	vector< vector<Point> > contours;
	vector<Rect> mser_bbox;

	detect_blobs(mGray, contours, mser_bbox, distance);

	vector<Rect*> type = filter_type(mser_bbox);

	vector<Rect> optRect = unique_blobs(type);

	return type;
}

vector<Rect> get_blobs(Mat mGray, int distance) {
	vector< vector<Point> > contours;
	vector<Rect> mser_bbox;

	detect_blobs(mGray, contours, mser_bbox, distance);

	vector<Rect> optRect;// = unique_blobs(filter_type(mser_bbox));

	return optRect;
}