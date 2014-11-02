#ifndef __PARSING__
#define __PARSING__

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <string>
#include "images.h"

using namespace std;

class ParsingAlgorithm {
    public:
    	ParsingAlgorithm(string aFilename) : filename(aFilename) {};
    	~ParsingAlgorithm(){};
		BayerImage ImageFromFile();
		void SaveQualityOfFilteredImage();
		void SaveImage(ColorImage anImage);
		void SaveComputationalTime();
		
	private:
		string filename;
		
		void saveImageChannel(string filename, const vector<vector<double> >& valuesToSave);
		string computationalTimeFilename();
		string savingImageFilename();
		string qualityFilename();        
};

#endif
