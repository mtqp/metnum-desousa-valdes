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

void BayerImage :: setPixel(int i, int j, double pixel){
	this->image[i][j] = pixel;
}

void BayerImage :: viewImage(){
	for (int i = 0; i < this->height; i++)
	{
		for (int j = 0; j < this->width; j++)
		{
			cout << this->image[i][j] << "\t";
		}
		cout << endl;
	}
}

ColorImage :: ColorImage(int width, int height){
	this->width = width;
	this->height = height;
	resizeTwoDimensionalVectorToWidthAndHeight(width, height, this->channel_r);
	resizeTwoDimensionalVectorToWidthAndHeight(width, height, this->channel_g);
	resizeTwoDimensionalVectorToWidthAndHeight(width, height, this->channel_b);
}
