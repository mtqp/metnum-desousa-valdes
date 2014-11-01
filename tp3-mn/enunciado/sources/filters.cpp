#include "filters.h"

using namespace std;

/******* Closest Neighbor *******/ 

ClosestNeighbor :: ~ClosestNeighbor(){}

double ClosestNeighbor :: FilterImage(){}


/******* Bilinear Interpolation *******/ 

BilinearInterpolation :: ~BilinearInterpolation(){}

double BilinearInterpolation :: FilterImage(){}


/******* Directional Interpolation *******/ 

DirectionalInterpolation :: ~DirectionalInterpolation(){}

double DirectionalInterpolation :: FilterImage(){}


/******* Malvar He Cutler Filter *******/ 

MalvarHeCutler :: ~MalvarHeCutler(){}

double MalvarHeCutler :: FilterImage(){}
