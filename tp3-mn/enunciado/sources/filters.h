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
        virtual double FilterImage() = 0;
        virtual ~Filter(){};
	
	protected:
        BayerImage bayerImage;
};

class ClosestNeighbor : public Filter {
    public:
        ClosestNeighbor(){}
        ~ClosestNeighbor();
    
        double FilterImage();
};

class BilinearInterpolation : public Filter {
    public:
        BilinearInterpolation(){}
        ~BilinearInterpolation();
    
        double FilterImage();
};

class DirectionalInterpolation : public Filter {
    public:
        DirectionalInterpolation(){}
        ~DirectionalInterpolation();
    
        double FilterImage();
};

class MalvarHeCutler : public Filter {
    public:
        MalvarHeCutler(){}
        ~MalvarHeCutler();
    
        double FilterImage();
};

#endif
