#include "filters.h"

using namespace std;

/******* Closest Neighbor *******/ 
//Consultar si hacemos experimento de Closest Neighbor comparando que vecino se toma (analisis cualitativo)

ClosestNeighbor :: ClosestNeighbor(BayerImage& aBayerImage) {
	this->bayerImage = aBayerImage;
}

ClosestNeighbor :: ~ClosestNeighbor(){}	

ColorImage ClosestNeighbor :: FilterImage()
{
	int newImageWidth = bayerImage.Width() - 2;
	int newImageHeight = bayerImage.Height() - 2;
	ColorImage filteredImage(newImageWidth, newImageHeight);
	
	for (int i = 1; i <= newImageHeight; i++)
	{
		for (int j = 1; j <= newImageWidth; j++)
		{
			int correctedRowIndexForFilteredImage = i-1;
			int correctedColIndexForFilteredImage = j-1;
			bool evenRow = i % 2 == 0;
			double blueNeighborPixel, redNeighborPixel, greenNeighborPixel;
			if (bayerImage.CurrentPixelIsGreen(i,j))
			{
				// Tomo los pixeles de la izquierda y arriba
				greenNeighborPixel = bayerImage.GetPixel(i, j);
				if (evenRow)
				{
					blueNeighborPixel = bayerImage.GetPixel(i, j-1);
					redNeighborPixel = bayerImage.GetPixel(i-1, j);
				} 
				else
				{
					blueNeighborPixel = bayerImage.GetPixel(i-1, j);
					redNeighborPixel = bayerImage.GetPixel(i, j-1);
				}
			}
			else
			{
				// Tomo los pixeles de la izquierda y arriba a la izquierda
				greenNeighborPixel = bayerImage.GetPixel(i, j-1);
				if (bayerImage.CurrentPixelIsRed(i,j))
				{
					redNeighborPixel = bayerImage.GetPixel(i, j);
					blueNeighborPixel = bayerImage.GetPixel(i-1, j-1);
				}
				else
				{
					redNeighborPixel = bayerImage.GetPixel(i-1, j-1);
					blueNeighborPixel = bayerImage.GetPixel(i, j);
				}
			}
			filteredImage.SetPixel(correctedRowIndexForFilteredImage, correctedColIndexForFilteredImage, redNeighborPixel, RED);
			filteredImage.SetPixel(correctedRowIndexForFilteredImage, correctedColIndexForFilteredImage, greenNeighborPixel, GREEN);
			filteredImage.SetPixel(correctedRowIndexForFilteredImage, correctedColIndexForFilteredImage, blueNeighborPixel, BLUE);	
		}
	}
	
	return filteredImage;
}

/******* Linear Interpolation Abstract Class *******/ 

double LinearInterpolation :: linearIntepolation(double value, double anotherValue){
	return (value + anotherValue)/2.0;
}

double LinearInterpolation :: interpolateVertical(int i, int j){
	double upNeighborPixel = bayerImage.GetPixel(i-1, j);
	double downNeighborPixel = bayerImage.GetPixel(i+1, j);
	
	return linearIntepolation(upNeighborPixel, downNeighborPixel);
}

double LinearInterpolation :: interpolateHorizontal(int i, int j){
	double leftNeighborPixel = bayerImage.GetPixel(i, j-1);
	double rightNeighborPixel = bayerImage.GetPixel(i, j+1);
	
	return linearIntepolation(leftNeighborPixel, rightNeighborPixel);
}

double LinearInterpolation :: interpolateCorners(int i, int j){
	double upLeftNeighborPixel = bayerImage.GetPixel(i-1, j-1);
	double upRightNeighborPixel = bayerImage.GetPixel(i-1, j+1);
	double downLeftNeighborPixel = bayerImage.GetPixel(i+1, j-1);
	double downRightNeighborPixel = bayerImage.GetPixel(i+1, j+1);
	
	double topInterpolationValue = linearIntepolation(upLeftNeighborPixel, upRightNeighborPixel);
	double downInterpolationValue = linearIntepolation(downLeftNeighborPixel, downRightNeighborPixel);
	
	return linearIntepolation(topInterpolationValue, downInterpolationValue);
}

double LinearInterpolation :: interpolateCross(int i, int j){	
	double verticalInterpolationValue = interpolateVertical(i, j);
	double horizontalInterpolationValue = interpolateHorizontal(i, j);
	
	return linearIntepolation(verticalInterpolationValue, horizontalInterpolationValue);
}

/******* Bilinear Interpolation *******/ 

BilinearInterpolation :: BilinearInterpolation(BayerImage& aBayerImage) {
	this->bayerImage = aBayerImage;
}

BilinearInterpolation :: ~BilinearInterpolation(){}

ColorImage BilinearInterpolation :: FilterImage(){
	int newImageWidth = bayerImage.Width() - 2;
	int newImageHeight = bayerImage.Height() - 2;
	ColorImage filteredImage(newImageWidth, newImageHeight);
	
	for (int i = 1; i <= newImageHeight; i++)
	{
		for (int j = 1; j <= newImageWidth; j++)
		{
			int correctedRowIndexForFilteredImage = i-1;
			int correctedColIndexForFilteredImage = j-1;
			bool evenRow = i % 2 == 0;
			double redChannelValue, greenChannelValue, blueChannelValue;
			if (bayerImage.CurrentPixelIsGreen(i,j))
			{
				// Tomo los pixeles de la izquierda y arriba
				greenChannelValue = bayerImage.GetPixel(i, j);
				if (evenRow)
				{
					// Rojos en eje vertical
					redChannelValue = interpolateVertical(i, j);
					blueChannelValue = interpolateHorizontal(i, j);
				} 
				else
				{
					// Azules en eje vertical
					blueChannelValue = interpolateVertical(i, j);
					redChannelValue = interpolateHorizontal(i, j);
				}
			}
			else
			{
				// Tomo la interpolación lineal horizontalmente y verticalmente
				greenChannelValue = interpolateCross(i, j);
				if (bayerImage.CurrentPixelIsRed(i,j))
				{					
					blueChannelValue = interpolateCorners(i, j);	
					redChannelValue = bayerImage.GetPixel(i, j);			
				}
				else
				{
					redChannelValue = interpolateCorners(i, j);
					blueChannelValue = bayerImage.GetPixel(i, j);
				}
			}
			filteredImage.SetPixel(correctedRowIndexForFilteredImage, correctedColIndexForFilteredImage, redChannelValue, RED);
			filteredImage.SetPixel(correctedRowIndexForFilteredImage, correctedColIndexForFilteredImage, greenChannelValue, GREEN);
			filteredImage.SetPixel(correctedRowIndexForFilteredImage, correctedColIndexForFilteredImage, blueChannelValue, BLUE);	
		}
	}
	
	return filteredImage;
}


/******* Directional Interpolation *******/ 

DirectionalInterpolation :: DirectionalInterpolation(BayerImage& aBayerImage) {
	this->bayerImage = aBayerImage;
}

DirectionalInterpolation :: ~DirectionalInterpolation(){}

ColorImage DirectionalInterpolation :: FilterImage(){}


/******* Malvar He Cutler Filter *******/ 

MalvarHeCutler :: MalvarHeCutler(BayerImage& aBayerImage) {
	this->bayerImage = aBayerImage;
}

MalvarHeCutler :: ~MalvarHeCutler(){}

ColorImage MalvarHeCutler :: FilterImage(){}
