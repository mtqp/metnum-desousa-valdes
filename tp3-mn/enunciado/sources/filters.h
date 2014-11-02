#ifndef __FILTERS__
#define __FILTERS__

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include "images.h"

using namespace std;

class Filter {
    public:
        virtual ColorImage FilterImage() = 0;
        virtual ~Filter(){};
	
	protected:
        BayerImage bayerImage;
        double interpolateVertical(int i, int j);
};

class ClosestNeighbor : public Filter {
    public:
        ClosestNeighbor(BayerImage& aBayerImage);
        ~ClosestNeighbor();
    
        ColorImage FilterImage();
};

class LinearInterpolation : public Filter {
	public:
		~LinearInterpolation(){}
		
	protected:
		double linearIntepolation(double value, double anotherValue);
		double interpolateVertical(int i, int j);
		double interpolateHorizontal(int i, int j);
		double interpolateCross(int i, int j);
		double interpolateCorners(int i, int j);
};

class BilinearInterpolation : public LinearInterpolation {
    public:
        BilinearInterpolation(BayerImage& aBayerImage);
        ~BilinearInterpolation();
    
        ColorImage FilterImage();
};

class DirectionalInterpolation : public Filter {
    public:
        DirectionalInterpolation(BayerImage& aBayerImage);
        ~DirectionalInterpolation();
    
        ColorImage FilterImage();
};

class MalvarHeCutler : public Filter {
    public:
        MalvarHeCutler(BayerImage& aBayerImage);
        ~MalvarHeCutler();
    
        ColorImage FilterImage();
};

#endif
