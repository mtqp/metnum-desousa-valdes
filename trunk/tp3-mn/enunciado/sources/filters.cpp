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


/******* Natural Spline Interpolation *******/ 

double SplineInterpolation :: horizontalGradient(int i, int j){
	double leftPixel = bayerImage.GetPixel(i, j-1);
	double rightPixel = bayerImage.GetPixel(i, j+1);
	return abs(leftPixel - rightPixel);
}

double SplineInterpolation :: verticalGradient(int i, int j){
	double topPixel = bayerImage.GetPixel(i-1, j);
	double downPixel = bayerImage.GetPixel(i+1, j);
	return abs(topPixel - downPixel);
}

vector<double> SplineInterpolation :: takeGreenFromRow(int i, int j, vector<int>& xValuesForData){
	int col = -1;
	vector<double> dataToInterpolate;
	if (bayerImage.CurrentPixelIsRed(i, j))
	{
		col = 0;
	}
	else if (bayerImage.CurrentPixelIsBlue(i, j))
	{
		col = 1;
	}
	
	if (col == -1) 
	{
		cout << "ERROR: takeGreenFromRow called from green pixel" << endl;
		exit(1);
	}
	
	for (; col < bayerImage.Width(); col = col + 2)
	{
		xValuesForData.push_back(col);
		dataToInterpolate.push_back(bayerImage.GetPixel(i, col));
	}
	return dataToInterpolate;
}

vector<double> SplineInterpolation :: takeGreenFromCol(int i, int j, vector<int>& xValuesForData){
	int row = -1;
	vector<double> dataToInterpolate;
	if (bayerImage.CurrentPixelIsRed(i, j))
	{
		row = 0;
	}
	else if (bayerImage.CurrentPixelIsBlue(i, j))
	{
		row = 1;
	}
	
	if (row == -1) 
	{
		cout << "ERROR: takeGreenFromCol called from green pixel" << endl;
		exit(1);
	}
	
	for (; row < bayerImage.Width(); row = row + 2)
	{
		xValuesForData.push_back(row);
		dataToInterpolate.push_back(bayerImage.GetPixel(row, j));
	}
	return dataToInterpolate;
}

double SplineInterpolation :: naturalCubicSplineInterpolation(int xValueForCurrentPixel, const vector<int>& x, const vector<double>& a){
	int n = (int)x.size() - 1;
	vector<double> b(n+1);
	vector<double> c(n+1);
	vector<double> d(n+1);
	
	// STEP 1
	vector<int> h(n);
	for (int i = 0; i <= n-1; i++)
	{
		h[i] = x[i+1] - x[i];
	}
	
	// STEP 2
	vector<double> alpha(n);
	for	(int i = 1; i <= n-1; i++)
	{
		alpha[i] = (3*(a[i+1] - a[i]) / h[i]) - (3*(a[i] - a[i-1]) / h[i-1]);
	}
	
	// Ya tenemos disponibles todas las ecuaciones pertenecientes al sistema tridiagonal edd que se forma
	
	
	// STEP 3
	vector<double> l(n+1);
	vector<double> z(n+1);
	vector<double> mu(n);
	l[0] = 1;
	mu[0] = 0;
	z[0] = 0;
	
	// STEP 4
	for	(int i = 1; i <= n-1; i++)
	{
		l[i] = 2*(x[i+1] - x[i-1]) - h[i-1]*mu[i-1];
		mu[i] = h[i]/l[i];
		z[i] = (alpha[i] - h[i-1]*z[i-1]) / l[i];
	}
	
	// STEP 5
	l[n] = 1;
	z[n] = 0;
	c[n] = 0;
	
	// STEP 6
	for (int j = n-1; j >= 0; j--)
	{
		c[j] = z[j] - mu[j]*c[j+1];
		b[j] = ((a[j+1] - a[j]) / h[j]) - h[j]*( (c[j+1] + 2*c[j]) / 3);
		d[j] = ( (c[j+1] - c[j]) / (3 * h[j]) );
	}
	
	// STEP 7 (output general: nosotros devolvemos solo el valor que nos interesa en el pixel)
	int indexToUse = floor((xValueForCurrentPixel - 1) / 2);
	return cubicFunction(double(xValueForCurrentPixel), (double)x[indexToUse], a[indexToUse], b[indexToUse], c[indexToUse], d[indexToUse]);
}

double SplineInterpolation :: cubicFunction(double x, double xj, double aj, double bj, double cj, double dj){
	return aj + bj * (x - xj) + cj * (x - xj) * (x - xj) + dj * (x - xj) * (x - xj) * (x - xj);
}
		
		
/******* Directional Interpolation *******/ 

DirectionalInterpolation :: DirectionalInterpolation(BayerImage& aBayerImage) {
	this->bayerImage = aBayerImage;
}

DirectionalInterpolation :: ~DirectionalInterpolation(){}

ColorImage DirectionalInterpolation :: FilterImage(){
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
				/// NO USAMOS SPLINES PARA EL ROJO Y EL AZUL (USAMOS LINEAR INTERPOLATION)
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
				///SPLINES PARA EL VERDE	
				bool interpolateRow = horizontalGradient(i, j) < verticalGradient(i, j);
				vector<double> dataToInterpolate;
				vector<int> xValuesForData;
				if (interpolateRow)
				{
					dataToInterpolate = takeGreenFromRow(i, j, xValuesForData);
					greenChannelValue = naturalCubicSplineInterpolation(j, xValuesForData, dataToInterpolate);			
				}
				else
				{
					dataToInterpolate = takeGreenFromCol(i, j, xValuesForData);
					greenChannelValue = naturalCubicSplineInterpolation(i, xValuesForData, dataToInterpolate);			
				}
	
				
				if (bayerImage.CurrentPixelIsRed(i,j))
				{	
					/// Blue queda con linearIntepolation
					blueChannelValue = interpolateCorners(i, j);	
					redChannelValue = bayerImage.GetPixel(i, j);			
				}
				else
				{
					/// Red queda con linearIntepolation
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


/******* Malvar He Cutler Filter *******/ 

MalvarHeCutler :: MalvarHeCutler(BayerImage& aBayerImage) {
	this->bayerImage = aBayerImage;
}

MalvarHeCutler :: ~MalvarHeCutler(){}

ColorImage MalvarHeCutler :: FilterImage(){}
