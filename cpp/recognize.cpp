#include "./recognize.h"
#include "./kenteken_nl.h"
#include "./util.h"

#ifdef _DEBUG
TextRecognitionModel recognizer = TextRecognitionModel("./traineddata1.onnx");
#else
TextRecognitionModel recognizer;
#endif

void init_recognition(string dictionary, string model) {

    recognizer = TextRecognitionModel(model);

    // Load vocabulary
    ifstream vocFile;
    vocFile.open(dictionary);
    CV_Assert(vocFile.is_open());
    String vocLine;
    vector<String> vocabulary;
    while (getline(vocFile, vocLine)) {
        vocabulary.push_back(vocLine);
    }
    recognizer.setVocabulary(vocabulary);
    recognizer.setDecodeType("CTC-greedy");

    double recScale = 1.0 / 127.5;
    Scalar recMean = Scalar(127.5);
    Size recInputSize = Size(100, 32);
    recognizer.setInputParams(recScale, recInputSize, recMean);
}

void init_recognition() {
    // Load vocabulary
    ifstream vocFile;
    vocFile.open("./alphabet0Z.txt");
    CV_Assert(vocFile.is_open());
    String vocLine;
    vector<String> vocabulary;
    while (getline(vocFile, vocLine)) {
        vocabulary.push_back(vocLine);
    }
    recognizer.setVocabulary(vocabulary);
    recognizer.setDecodeType("CTC-greedy");

    double recScale = 1.0 / 127.5;
    Scalar recMean = Scalar(127.5);
    Size recInputSize = Size(100, 32);
    recognizer.setInputParams(recScale, recInputSize, recMean);
}

RecognitionResult optimize_car_licence(RecognitionResult license) {
    replace(license.first.begin(), license.first.end(), 'F', 'R');
    return license;
}

string recognize(Mat m) {
    return recognizer.recognize(m);
}

string recognize(Mat image, Rect rect) {
    if (rect.x >= 0 and image.cols >= rect.x + rect.width
        and rect.y >= 0 and image.rows >= rect.y + rect.height)
        return recognize(image(rect));
    else return "";
}

string recognize_part(Mat image, Rect rect) {
    string ret;
    if (is_inside(rect, image))
        ret = recognize(image, rect);

    return ret;

}

RecognitionResult recognize(Mat image, const vector<Rect *> boxes, int first, int second, int third, int fourth) {

    RecognitionResult result;
    vector<Rect*> box;

    for (int n = 0; n < first; n++) {
        if(boxes.size() > n)
            box.insert(box.end(), boxes[n]);
    }

    result.first = recognize_part(image, bounding_rect(box));

    box.clear();

    for (int n = first; n < first + second + fourth; n++) {
        if (boxes.size() > n)
            box.insert(box.end(), boxes[n]);
    }

    if(second > 0)
        result.second = recognize_part(image, bounding_rect(box));

    box.clear();

    for (int n = first + second + fourth; n < first + second + third + fourth; n++) {
        if (boxes.size() > n)
            box.insert(box.end(), boxes[n]);
    }

    if(third > 0)
        result.third = recognize_part(image, bounding_rect(box));
     
    if (is_car_licence(result))
        result = optimize_car_licence(result);

    return result;
}