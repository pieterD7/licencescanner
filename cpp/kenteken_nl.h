/*
	Since licence plate series 4 (1978) no vowels are used in licences besides O. The vocabulary 
	used by the recognition doens't contain the vowels (including O) so all licenceplates starting with
	series 4, besides those of opleggers which start with an O (most likely to become a V after 
	recogntion), can be scanned. Although a scootmobiel has a plate for the insurance these may look like
	a licence but aren't and these cannot be scanned (will be scanned as a two line car licence which is an 
	exception.)
*/

#pragma once
#include "./licence.h"

bool is_car_licence(RecognitionResult test);