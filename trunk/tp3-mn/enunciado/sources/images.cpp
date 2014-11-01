#include "images.h"

using namespace std;

BayerImage :: BayerImage(int width, int height){
	this->width = width;
	this->height = height;
	this->image = vector< vector<double>(width) >(height);
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
	this->channel_r = vector< vector<double>(width) >(height);
	this->channel_g = vector< vector<double>(width) >(height);
	this->channel_b = vector< vector<double>(width) >(height);
}
