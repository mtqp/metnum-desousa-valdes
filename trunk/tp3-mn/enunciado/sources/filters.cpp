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





/******* Bilinear Interpolation *******/ 

BilinearInterpolation :: ~BilinearInterpolation(){}

ColorImage BilinearInterpolation :: FilterImage(){}


/******* Directional Interpolation *******/ 

DirectionalInterpolation :: ~DirectionalInterpolation(){}

ColorImage DirectionalInterpolation :: FilterImage(){}


/******* Malvar He Cutler Filter *******/ 

MalvarHeCutler :: ~MalvarHeCutler(){}

ColorImage MalvarHeCutler :: FilterImage(){}
