#ifndef __IMAGES__
#define __IMAGES__

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <string>

using namespace std;

class BayerImage {
    public:
    	BayerImage(int width, int height);
    	BayerImage();
    	~BayerImage();
    	void setPixel(int i, int j, double pixel);
    	void viewImage();
		
	private:
		int width;
		int height;
		vector<vector<double> > image;
        
};


class ColorImage {
    public:
    	ColorImage(int width, int height);
    	~ColorImage();
		
	private:
		int width;
		int height;
		vector<vector<double> > channel_r;
		vector<vector<double> > channel_g;
		vector<vector<double> > channel_b;
};

#endif
