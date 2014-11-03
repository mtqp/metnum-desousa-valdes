#ifndef __MAIN__
#define __MAIN__

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <string>
#include "images.h"
#include "parsing.h"
#include "filters.h"

using namespace std;

enum FilterAlgorithmType { CLOSEST_NEIGHBOR, BILINEAR_INTERPOLATION, DIRECTIONAL_INTERPOLATION, MALVAR_HE_CUTLER }; // CLOSEST_NEIGHBOR = 0, BILINEAR_INTERPOLATION = 1, DIRECTIONAL_INTERPOLATION = 2, MALVAR_HE_CUTLER = 3

Filter* CreateFilterTypeFromParameter(FilterAlgorithmType filterAlgorithmType, BayerImage& bayerImage);

#endif
