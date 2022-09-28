#include "./analyse.h"
#include "./util.h"
#include "./characteristic.h"
#include "./neighbours.h"
#include "./candidates.h"
#include "./recognize.h"
#include "./detect.h"
#include "./scanimage.h"

// Colors are BGRA
Scalar green = Scalar(0, 255, 0, 255);
Scalar orange = Scalar(0, 165, 255, 255);
Scalar yellow = Scalar(0, 255, 255, 128);
Scalar red = Scalar(0, 0, 255, 255);

vector<Rect*> type;
vector<Rect*> type2;
vector<vector<Rect*>> bxs;
vector<MatrixElement*> dmatrix;
vector<vector<Point>> contours;
vector<Rect> mser_bbox;
vector<vector<Point>> contours_ch;
vector<Rect> mser_bbox_ch;
vector<Rect*> ch;


vector<Rect*> get_type(){
    return type;
}

vector<vector<Rect*>> get_bxs(){
    return bxs;
}

vector<MatrixElement*> get_dmatrix(){
    return dmatrix;
}


void display_output( Mat mColor, vector<Rect> mser_bbox, 
	vector<Rect *> type, vector<MatrixElement *> dmatrix,
	vector<vector<Rect *>> bxs, vector<Rect*> ch, String result) {

	if (DUMP)
	{

		//drawContours(mColor, contours, -1, Scalar(0, 165, 255, 255), 1, LINE_8);

		if (DUMP_BLOBS)
		{
			// Dump all blobs
			for (int i = 0; i < mser_bbox.size(); i++)
			{
				rectangle(mColor, mser_bbox[i], green, 1, LINE_8);
			}
		}

		if (DUMP_TYPE)
		{

			// Dump type
			for (int i = 0; i < type.size(); i++)
			{
				rectangle(mColor, *type[i], red, 1, LINE_8);
			}
		}

		if (DUMP_MATRIX)
		{
			// Dump matrix
			for (int i = 0; i < dmatrix.size(); i++)
			{
				MatrixElement* el = dmatrix[i];
				Rect rect = *type[el->bb];
				Rect rect2 = *type[el->b];
				rectangle(mColor, rect, yellow, 2, LINE_8, 0);
				rectangle(mColor, rect2, yellow, 2, LINE_8, 0);
			}
		}

		// Indicate characteristics
		for (int i = 0; i < ch.size(); i++) {
			rectangle(mColor, *ch[i], orange, 2, LINE_8, 0);
		}

		if (DEBUG) {

			cout << "Blobs : " << mser_bbox.size() << endl;
			cout << "Type : " << type.size() << endl;
			cout << "Matrix : " << dmatrix.size() << endl;
			cout << "Ch : " << ch.size() << endl;
			cout << "Boxes of licence : ";
			if (bxs.size() > 0)
				cout << bxs[0].size() << endl;
			else
				cout << "0" << endl;

			cout << "Angle : " << get_rotation_angle() << endl;

			cout << "First part : " << get_first() << endl;
			cout << "Second part : " << get_second() << "+" << get_fourth() << endl;
			cout << "Third part : " << get_third() << endl;

			cout << "Result : " << result << endl;

		}

		// Display a rectangle around found licence plate
		if (bxs.size() > 0) {
			Rect r = bounding_rect(bxs[0]);
			rectangle(mColor, r, green, 2, LINE_8);
		}
	}
}

double calc_scale(Mat mGrayIn) {

	// scale to longest side = 1600
	double scale = 1.0;
	int width = mGrayIn.cols;
	int height = mGrayIn.rows;

	if (height > width) {
		if (height > 1600.0) {
			scale = 1600.0 / height;
		}
	}
	else if (width > 1600) {
		scale = 1600.0 / width;
	}
	return scale;
}

void prepare_frame(Mat &mGray, double scale) {
	resize(mGray, mGray, Size(), scale, scale);
}

void detect(Mat mGray) {
	vector<Rect*> optRect;

	detect_blobs(mGray, contours, mser_bbox);

	optRect = filter_type(mser_bbox);

	type = unique_blobs(optRect);

}

void detect_ch(Mat mGray) {
	detect_blobs_ch(mGray, contours_ch, mser_bbox_ch);
}

string analyse_mat(Mat mGray){

		detect(mGray);

		sort(type.begin(), type.end(), cmp_left_to_right);

		dmatrix = make_matrix(type, mGray.cols, mGray.rows);

		bxs = get_candidates(type, dmatrix);

		RecognitionResult result;

		Mat scanImage;

		if (bxs.size() > 0) {
			Rect area = bounding_rect(bxs[0]);

			if ((area & Rect(0, 0, mGray.cols, mGray.rows)) == area and
				area.width > 2 and area.height > 2)
				detect_ch(mGray(area));

			vector<Characteristic> chr = characteristic(contours_ch, mser_bbox_ch);

			ch = unique_blobs(filter_ch(chr, dmatrix, type, Point(area.x, area.y)));

			evaluate_ch(ch, bxs[0], get_has_two_lines(), get_size_first_line());

			if (get_has_two_lines() and ch.size() > 0) {
				vector<vector<Point>> contours;

				scanImage = make_scanimage(mGray, bxs[0], contours, -1 * get_rotation_angle());

				result = recognize(scanImage, 
					boxes_to_boxes(bxs[0], -1 * get_rotation_angle()), 
					get_first(), get_second(), get_third(), get_fourth()
				);

			}
			else
				result = recognize(mGray, bxs[0], get_first(), get_second(), get_third(), get_fourth());

            return result.toString();
		}
        return "";
}

Mat analyse(Mat mGray, Mat mColor, int preview)
{

	try {

		init_recognition();

		double scale = calc_scale(mGray);

		prepare_frame(mGray, scale);

		threshold(mGray, mGray, 40, 255, 3);

		Rect roi = Rect(0, 0, mGray.cols, mGray.rows);

		string result = analyse_mat(mGray(roi));

		cvtColor(mGray, mColor, COLOR_GRAY2RGBA, 0);

		//if(get_has_two_lines())
		//	mColor = scanImage;

		display_output(mColor(roi), mser_bbox, type, dmatrix, bxs, ch, result);

		return mColor;
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
		return mGray;
	}
}
