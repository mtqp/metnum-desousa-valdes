#ifndef __IMAGES__
#define __IMAGES__

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <string>

using namespace std;

enum ChannelType { RED, GREEN, BLUE }; // RED = 0, GREEN = 1, BLUE = 2

void resizeTwoDimensionalVectorToWidthAndHeight(int aWidth, int aHeight, vector< vector<double> >& aVector);

class BayerImage {
    public:
    	BayerImage(int width, int height);
    	BayerImage(){};
    	~BayerImage(){};
    	int Width();
    	int Height();
    	bool CurrentPixelIsRed(int i, int j);
    	bool CurrentPixelIsGreen(int i, int j);
    	bool CurrentPixelIsBlue(int i, int j);
    	void SetPixel(int i, int j, double pixel);
    	double GetPixel(int i, int j);
    	void ViewImage(); // only debug
		
	private:
		int width;
		int height;
		vector<vector<double> > image;
};


class ColorImage {
    public:
    	ColorImage(int width, int height);
    	ColorImage(){};
    	~ColorImage(){};
		int Width();
    	int Height();
    	const vector<vector<double> >& ChannelR();
    	const vector<vector<double> >& ChannelG();
    	const vector<vector<double> >& ChannelB();
    	void SetPixel(int i, int j, double pixel, ChannelType channel);
		
	private:
		int width;
		int height;
		vector<vector<double> > channel_r;
		vector<vector<double> > channel_g;
		vector<vector<double> > channel_b;
};

#endif
