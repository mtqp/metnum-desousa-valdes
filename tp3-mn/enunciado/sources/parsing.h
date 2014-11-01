#ifndef __PARSING__
#define __PARSING__

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <string>
#include "images.h"

using namespace std;

class ParsingAlgorithm {
    public:
    	ParsingAlgorithm(string aFilename) : filename(aFilename) {};
    	~ParsingAlgorithm();
		BayerImage imageFromFile();
		void saveQualityOfFilteredImage();
		void saveImage(ColorImage anImage);
		void saveComputationalTime();
		
	private:
		string filename;
		
		string computationalTimeFilename();
		string savingImageFilename();
		string qualityFilename();        
};

#endif
