
#include "./candidates.h"
#include "./neighbours.h"
#include "./util.h"

bool hasTwoLines = false;
int sizeFirstLIne = 0;
int first;
int second;
int third;
int fourth;

bool get_has_two_lines() {
	return hasTwoLines;
}

int get_size_first_line() {
	return sizeFirstLIne;
}

int get_first() {
	return first;
}

int get_second() {
	return second;
}

int get_third() {
	return third;
}

int get_fourth() {
	return fourth;
}

int find_neighbour(vector<MatrixElement*> dmatrix, int b) {
	
	for (int n = 0; n < dmatrix.size(); n++) {
		if (dmatrix[n]->b == b)
			return dmatrix[n]->bb;
	}
	return -1;
}

vector<int> on_same_line(vector<MatrixElement*> dmatrix, int b) {
	vector<int> ret = {b};
	int nb = b;

	for (int n = 0; n < 6; n++) {
		nb = find_neighbour(dmatrix, nb);
		if (nb == -1)
			break;
		else
			ret.insert(ret.end(), nb);
	}

	return ret;
}

bool on_line_below(int y, int boxY, int height) {
	return (boxY < y - height and boxY > y - 1.5 * height);
}

bool on_line_above(int y, int boxY, int height) {
	return boxY > y + height and boxY < y + 1.5 * height;
}

int leftmost(vector<Rect*> boxes, vector<int> m) {
	int minX = 99999;
	int left = -1;
	for (int n = 0; n < m.size(); n++) {
		if (boxes[m[n]]->x < minX) {
			minX = boxes[m[n]]->x;
			left = n;
		}
	}
	return left;
}

int next_block(vector<Rect*> boxes, vector<MatrixElement*> dmatrix,  int b, bool skip, Rect* lastBlock) {
	if (skip) {
		int r = int(dmatrix.size());
		for (int n = b + 1; n < dmatrix.size(); n++) {
			Rect* rect = boxes[dmatrix[n]->b];
			if (! near(rect->y, lastBlock->y, AB * lastBlock->height)) {
				r = n;
				break;
			}
		}
		return r;
	}
	return ++b;
}

int next_block(vector<Rect*> boxes, vector<MatrixElement*> dmatrix, const char* direction, int b, bool skip, Rect* lastBlock) {
	if (skip) {
		int r = int(dmatrix.size());
		for (int n = b; n < dmatrix.size(); n++) {
			if ((string)direction == "h") {
				Rect* rect = boxes[dmatrix[n]->b];
				if (near(rect->y, lastBlock->y, AB * lastBlock->height)) {
					r = n;
					break;
				}
			}
			else if ((string)direction == "v") {
				Rect* rect = boxes[dmatrix[n]->b];
				if (near(rect->x, lastBlock->x, AB * lastBlock->width)) {
					r = n;
					break;
				}
			}
		}
		return r;
	}
	return ++b;
}

bool ch_inside(vector<Rect*> ch, Rect box) {

	bool ok = false;
	for (int n = 0; n < ch.size(); n++) {
		if (box.contains(Point(ch[n]->x, ch[n]->y)))
			ok = true;
	}
	return ok;
}

/*
	Position of ch is after returned index
*/
int position_of_ch(vector<Rect*> boxes, Rect* ch) {
	int ret = 0;
	for (int n = 1; n < boxes.size(); n++) {
		if (ch->x > boxes[n - 1]->x and ch->x < boxes[n]->x and near(ch->y, boxes[n]->y, boxes[n]->height / 2)) {
			ret = n;
		}
	}
	return ret;
}

vector<vector<Rect*>> get_candidates(vector<Rect*> boxes, vector<MatrixElement*> dmatrix) {
	vector<vector<Rect*>> ret;
	vector<Rect*> found1;

	sizeFirstLIne = 0;
	hasTwoLines = false;

	bool skipLine = false;
	Rect* lastBlockInLine{};

	for (int b = 0; b < dmatrix.size();  b = next_block(boxes, dmatrix, b, skipLine, lastBlockInLine)) {
		vector<Rect*> found2;
		found2.insert(found2.end(), boxes[dmatrix[b]->b]);
		int n = dmatrix[b]->b;
		skipLine = false;

		while (n > -1) {
			int m = find_neighbour(dmatrix, n);
			if (m > -1) {
				found2.insert(found2.end(), boxes[m]);
				n = m;
			}
			else
				n = m;
		}

		if (found2.size() > MAX_PARTS) {
			skipLine = true;
			lastBlockInLine = boxes[dmatrix[b]->b];
		}
		else if (found2.size() >= N_PARTS and 
			found1.size() < found2.size()) {
			found1 = found2;
			break;
		}
	}

	// On two lines?
	// X-XX / -XXX
	// XX- / XX-XX
	// X-XX / X-XXX
	// XXX / -X-XX
	if (found1.size() == 0) {

		vector<Rect*> foundL1;
		vector<Rect*> foundL2;

		bool firstLineIsAbove = false;


		int yL1 = 0;
		int hL1 = 0;

		for (int b = 0; b < dmatrix.size(); b++) {
			foundL1.insert(foundL1.end(), boxes[dmatrix[b]->b]);
			int n = dmatrix[b]->b;
			hL1 = boxes[n]->height;

			while (n > -1) {
				int m = find_neighbour(dmatrix, n);
				if (m > -1) {
					hL1 = boxes[n]->height;
					yL1 = boxes[n]->y;
					foundL1.insert(foundL1.end(), boxes[m]);
					n = m;
				}
				else
					n = -1;
			}

			if (foundL1.size() > 1) {
				for (int b = 0; b < dmatrix.size(); b++) {

					if (near(boxes[dmatrix[b]->b]->height, hL1, AB * hL1)) {

						if (on_line_above(yL1, boxes[dmatrix[b]->b]->y, boxes[dmatrix[b]->b]->height))
							firstLineIsAbove = true;
						else if (on_line_below(yL1, boxes[dmatrix[b]->b]->y, boxes[dmatrix[b]->b]->height))
							firstLineIsAbove = false;
						else
							continue;

						foundL2.insert(foundL2.end(), boxes[dmatrix[b]->b]);
						int n = dmatrix[b]->b;
						while (n > -1) {

							int m = find_neighbour(dmatrix, n);
							if (m > -1) {
								if (near(hL1, boxes[m]->height, AB * hL1))
									foundL2.insert(foundL2.end(), boxes[m]);
								n = m;
							}
							else
								n = m;

						}
						break;
					}
				}
			}

			if (foundL2.size() + foundL1.size() < N_PARTS ||
				foundL2.size() + foundL1.size() > MAX_PARTS) {
				foundL1.clear();
				foundL2.clear();
			}
			else {
				hasTwoLines = true;
				if (firstLineIsAbove)
					sizeFirstLIne = (int)foundL1.size();
				else
					sizeFirstLIne = (int)foundL2.size();
				break;
			}
		}

		if (firstLineIsAbove)
			found1 = combine(foundL1, foundL2);
		else
			found1 = combine(foundL2, foundL1);
	}

	if(found1.size() > 0)
		ret.insert(ret.end(), found1);

	return ret;
}

void evaluate_ch(vector<Rect*> ch, vector<Rect*> found1, bool hasTwoLines, int sizeFirstLine) {
	
	first = 0;

	second = 0;

	third = 0;

	fourth = 0;

	if (hasTwoLines) {
		sort(ch.begin(), ch.end(), cmp_top_to_bottom);
	}
	else
		sort(ch.begin(), ch.end(), cmp_left_to_right);


	/*
		If there are two lines and the first line doesn't have a ch, the first part is the first line,
		otherwise it is the part left of the first ch.
	*/
	if (hasTwoLines and ch.size() == 1) {
		first = sizeFirstLine;
	}
	else if (ch.size() > 0) {
		first = position_of_ch(found1, ch[0]);
	}
	else if (found1.size() >= N_PARTS) {
		first = 3;
		second = 3;
	}

	/*
		If there are two lines and the first has a ch the second part is the part to the right of the first ch,
		otherwise it is the part to the right of the first ch.
	*/
	if (ch.size() > 0 and hasTwoLines and position_of_ch(found1, ch[0]) < sizeFirstLine) {
		second = position_of_ch(found1, ch[0]);
	}
	else {
		if (ch.size() > 1)
			second = position_of_ch(found1, ch[1]) - first;
		else if (ch.size() > 0)
			second = position_of_ch(found1, ch[0]) - first;
	}

	/*
		The third part is the part to the right of the last ch
	*/
	if (ch.size() > 0)
		third = 6 - position_of_ch(found1, ch[ch.size() - 1]);


	/*
	* Suffix to the second part.
	* If there are two lines and the first line has a ch, the fourth part is the part to the left of the second ch.
	*/

	if (hasTwoLines and ch.size() > 1 and position_of_ch(found1, ch[0]) < sizeFirstLine)
		fourth = N_PARTS - first - second - third;;

}