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

class SplineInterpolation : public LinearInterpolation {
	public:
		~SplineInterpolation(){}
		
	protected:
		double horizontalGradient(int i, int j);
		double verticalGradient(int i, int j);
		vector<double> takeGreenFromRow(int i, int j, vector<int>& xValuesForData);
		vector<double> takeGreenFromCol(int i, int j, vector<int>& xValuesForData);
		double naturalCubicSplineInterpolation(int xValueForCurrentPixel, const vector<int>& x, const vector<double>& a);
		double cubicFunction(double x, double xj, double aj, double bj, double cj, double dj);
};

class DirectionalInterpolation : public SplineInterpolation {
    public:
        DirectionalInterpolation(BayerImage& aBayerImage);
        ~DirectionalInterpolation();
    
        ColorImage FilterImage();
};

class MalvarHeCutlerInterpolation : public LinearInterpolation {
	public:
		~MalvarHeCutlerInterpolation(){}
		
	protected:
		double gradientCorrection(int i, int j);
};


class MalvarHeCutler : public MalvarHeCutlerInterpolation {
    public:
        MalvarHeCutler(BayerImage& aBayerImage, double alpha);
        ~MalvarHeCutler();
    
        ColorImage FilterImage();
	
	private:
		double alpha;
};

#endif
