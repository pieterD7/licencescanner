#pragma once

#ifdef _DEBUG
// cout some info about the scan?
#define DEBUG true
#else

#define DEBUG false
#endif // _DEBUG

// All letters and digits have the same height.
// Allow difference in size
#define AB 15.0 / 100

// Maximum angle between two characters
#define AB_DEGREES 2

// Filter what could be characters. 
#define TYPE_MIN_RATIO 0.85

#define TYPE_MAX_RATIO 12.0

// Orientations of the device. This will set the preference for an horizotal or vertical number
#define LANDSCAPE 0

#define PORTRAIT 1

// The sequence to find has n parts (minimal)
#define N_PARTS 6

// Discard a line with maxParts + 1 blocks
#define MAX_PARTS 8

// Multiplier for max distance between neighbours 
#define MAX_DIST_H 1.5

#define DUMP 			true	// The mains for dumping in the CL-version

#define DUMP_BLOBS 		false	// Dump blobs

#define DUMP_TYPE 		true	// Show rectangle around characters

#define DUMP_MATRIX 	true	// Dump last matrix

#define TRY_ONCE 		false	// Have only one try for an horizotal or vertical number

// Max settings
#define MAX_SIZE_MATRIX		250

#define MAX_SIZE_TYPE		500

// Constants for what to view in CL-version
#define SHOW_SCAN_FRAME		1

#define SHOW_CAMERA_VIEW	2

