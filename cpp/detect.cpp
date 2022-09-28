#include "./detect.h"
#include "./constants.h"
#include "./util.h"

void detect_blobs(Mat mGray, vector< vector<Point> >& contours, vector<Rect>& mser_bbox) {
	int delta = 5;
	int minimal_area = 5;
	int max_area = 9000;
	double max_variation = 0.08;
	Ptr<MSER> detector = MSER::create(delta, minimal_area, max_area, max_variation);
//	Ptr<MSER> detector = MSER::create();

	detector->detectRegions(mGray, contours, mser_bbox);
}

void detect_blobs_ch(Mat mGray, vector< vector<Point> >& contours, vector<Rect>& mser_bbox) {
	int delta = 2;
	int minimal_area = 5;
	int max_area = 300;
	double max_variation = 0.07;
	Ptr<MSER> detector = MSER::create(delta, minimal_area, max_area, max_variation);

	detector->detectRegions(mGray, contours, mser_bbox);

}

// Take largest rectangle at a position
vector<Rect*> unique_blobs(vector<Rect*> blobs) {

	vector<Rect*> opt;
	vector<int> had;

	for (int n = 0; n < blobs.size(); n++) {
		Rect* r = blobs[n];
		for (int m = n + 1; m < blobs.size(); m++) {
			Rect* rr = blobs[m];
			if (rectangles_overlap(r, rr)){
				
				//r = &add(*rr, *r);

				r->x = min(blobs[n]->x, blobs[m]->x);
				r->y = min(blobs[n]->y, blobs[m]->y);
				r->width = max(blobs[n]->width, blobs[m]->width);
				r->height = max(blobs[n]->height, blobs[m]->height);

				had.insert(had.end(), m);
			}
		}

		if (find(had.begin(), had.end(), n) == had.end())
			opt.insert(opt.end(), r);
	}
	return opt;
}


// Take largest rectangle at a position
vector<Rect*> unique_blobs(vector<Characteristic> blobs) {

	vector<Rect*> opt;
	vector<int> had;

	for (int n = 0; n < blobs.size(); n++) {
		Rect* r = blobs[n].rect;
		for (int m = n + 1; m < blobs.size(); m++) {
			Rect* rr = blobs[m].rect;
			if (rectangles_overlap(r, rr)) {

				//Rect ri = *rr | *r;
				//r = &ri;

				r->x = min(blobs[n].rect->x, blobs[m].rect->x);
				r->y = min(blobs[n].rect->y, blobs[m].rect->y);
				r->width = max(blobs[n].rect->width, blobs[m].rect->width);
				r->height = max(blobs[n].rect->height, blobs[m].rect->height);

				had.insert(had.end(), m);
			}
		}

		if (find(had.begin(), had.end(), n) == had.end())
			opt.insert(opt.end(), r);
	}
	return opt;
}

// Filter typography by aspect ratio
bool is_type(float width, float height) {
	if (height / width > TYPE_MIN_RATIO and
		height / width < TYPE_MAX_RATIO)
	{
		return true;
	}
	return false;
}

vector<Rect* > filter_type(vector<Rect>& blobs) {

	vector<Rect*> type;

	for (int i = 0; i < blobs.size(); i++)
	{
		if (is_type(float(blobs[i].width), float(blobs[i].height))
			and type.size() < MAX_SIZE_TYPE)
			
			type.insert(type.end(), &blobs[i]);
	}
	return type;
}
