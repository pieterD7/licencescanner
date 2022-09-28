
#include <filesystem>
#include "./constants.h"
#include "./analyse.h"
#include "./output.h"


const char* keys =
"{ help h ?| | Print this help message. }"
"{ @input | | Input image (path and) filename or directory. }"
"{ @output | | Output directory. }"
"{ view v |scan| scan: view scan; camera: view camera. }";

bool processFile(string inputFile, string outDir, int view) {

	clock_t tStart = clock();

	Mat image;
	Mat image2;
	Mat output;
	image = imread(inputFile, IMREAD_GRAYSCALE);
	image2 = imread(inputFile, IMREAD_COLOR);

	if (image.empty()) // Check for invalid input
	{
		cout << "Could not open or find image " << inputFile << endl;
		return false;
	}

	// Process
	output = analyse(image, image2, view);

	// Make the output fit on my laptop
	if (output.cols > 1200 or output.rows > 1200)
		resize(output, output, Size(), 0.5, 0.5);

	namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.

	imshow("Display window", output); // Show our image inside it.

	printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	waitKey(0); // Wait for a keystroke in the window
	

	image.release();
	image2.release();
	output.release();

	return true;

}

int main(int argc, char** argv)
{
	utils::logging::setLogLevel(utils::logging::LogLevel::LOG_LEVEL_ERROR);

	CommandLineParser parser(argc, argv, keys);
	parser.about("Scan the ISO6346 number on a picture of a container.");

	String inputFile = parser.get<String>(0);

	if (argc == 1 || parser.has("help") || inputFile == "")
	{
		parser.printMessage();
		return -1;
	}

	string outDir = parser.get<String>(1);


	int view = SHOW_SCAN_FRAME;
	if (parser.get<String>("view") == "camera")
		view = SHOW_CAMERA_VIEW;

	bool processDir = std::filesystem::is_directory(inputFile);

	if (processDir) {
		for (const auto& entry : std::filesystem::directory_iterator(inputFile)) {
			if (!entry.is_directory())
				processFile(entry.path().string(), outDir, view);
		}
	}
	else {
		return processFile(inputFile, outDir, view);
	}

	return 0;
}