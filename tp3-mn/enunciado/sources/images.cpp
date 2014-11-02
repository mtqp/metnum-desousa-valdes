#include "images.h"

using namespace std;

void resizeTwoDimensionalVectorToWidthAndHeight(int aWidth, int aHeight, vector< vector<double> >& aVector){
	aVector.resize(aHeight);
	for (int i = 0; i < aHeight; i++)
		aVector[i].resize(aWidth);
}

BayerImage :: BayerImage(int width, int height){
	this->width = width;
	this->height = height;
	resizeTwoDimensionalVectorToWidthAndHeight(width, height, this->image);
}

int BayerImage :: Width(){ return width; }

int BayerImage :: Height(){ return height; }

void BayerImage :: SetPixel(int i, int j, double pixel){
	this->image[i][j] = pixel;
}

void BayerImage :: ViewImage(){
	for (int i = 0; i < this->height; i++)
	{
		for (int j = 0; j < this->width; j++)
		{
			cout << this->image[i][j] << "\t";
		}
		cout << endl;
	}
}

double BayerImage :: GetPixel(int i, int j){
	return this->image[i][j];
}

bool BayerImage :: CurrentPixelIsRed(int i, int j){
	bool evenRow = i % 2 == 0, evenCol = j % 2 == 0;
	return !evenRow && !evenCol;
}

bool BayerImage :: CurrentPixelIsGreen(int i, int j){
	bool evenRow = i % 2 == 0, evenCol = j % 2 == 0;
	return (evenRow && !evenCol) || (!evenRow && evenCol);	
}

bool BayerImage :: CurrentPixelIsBlue(int i, int j){
	return !CurrentPixelIsGreen(i, j) && !CurrentPixelIsRed(i, j);
}

ColorImage :: ColorImage(int width, int height){
	this->width = width;
	this->height = height;
	resizeTwoDimensionalVectorToWidthAndHeight(width, height, this->channel_r);
	resizeTwoDimensionalVectorToWidthAndHeight(width, height, this->channel_g);
	resizeTwoDimensionalVectorToWidthAndHeight(width, height, this->channel_b);
}

int ColorImage :: Width(){ return width; }

int ColorImage :: Height(){ return height; }

const vector<vector<double> >& ColorImage :: ChannelR(){
	return channel_r;
}
const vector<vector<double> >& ColorImage :: ChannelG(){
	return channel_g;
}

const vector<vector<double> >& ColorImage :: ChannelB(){
	return channel_b;
}

void ColorImage :: SetPixel(int i, int j, double pixel, ChannelType channel){
	switch (channel){
		case RED:{
			this->channel_r[i][j] = pixel;
			break;
		}
		
		case GREEN:{
			this->channel_g[i][j] = pixel;
			break;
		}
		
		case BLUE:{
			this->channel_b[i][j] = pixel;
			break;
		}
		
		default:{
			cout << "ERROR: haven't specified channel type." << endl;
			exit(1);
			break;
		}
	}
}
