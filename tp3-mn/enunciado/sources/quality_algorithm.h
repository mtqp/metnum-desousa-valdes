#ifndef __QUALITY_ALGORITHMS__
#define __QUALITY_ALGORITHMS__

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

class QualityAlgorithm {
    public:
        virtual double QualifyImage() = 0;
        virtual ~QualityAlgorithm(){};
	
	protected:
        ColorImage image;
};

class PSNR : public QualityAlgorithm {
    public:
        PSNR(){}
        ~PSNR();
    
        double QualifyImage();
};

class SSIM : public QualityAlgorithm {
    public:
        SSIM(){}
        ~SSIM();
    
        double QualifyImage();
};

#endif
