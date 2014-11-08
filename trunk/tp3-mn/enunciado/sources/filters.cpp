#include "filters.h"

using namespace std;

/************ Filter ************/

uint64 Filter :: ComputationalTime(){
	return computationalTime;
}

double Filter :: getCompTimeAndGreenChannelInRedAndBluePixels(int i, int j){
	uint64 start = rdtsc();
	double greenChannelValue = getGreenChannelInRedAndBluePixels(i,j);
	uint64 finish = rdtsc();
	uint64 total = finish - start;
	computationalTime = computationalTime + total;
	return greenChannelValue;
}

ColorImage Filter :: FilterImage(){
	int newImageWidth = bayerImage.Width() - distanceFromPixelStartToBorder * 2;
	int newImageHeight = bayerImage.Height() - distanceFromPixelStartToBorder * 2;
	ColorImage filteredImage(newImageWidth, newImageHeight);
	
	for (int i = distanceFromPixelStartToBorder; i < bayerImage.Height() - distanceFromPixelStartToBorder; i++)
	{
		for (int j = distanceFromPixelStartToBorder; j < bayerImage.Width() - distanceFromPixelStartToBorder; j++)
		{
			int correctedRowIndexForFilteredImage = i - distanceFromPixelStartToBorder;
			int correctedColIndexForFilteredImage = j - distanceFromPixelStartToBorder;
			double redChannelValue, greenChannelValue, blueChannelValue;
			if (bayerImage.CurrentPixelIsGreen(i,j))
			{
				greenChannelValue = bayerImage.GetPixel(i, j);
				blueChannelValue = getBlueChannelInGreenPixel(i, j);
				redChannelValue = getRedChannelInGreenPixel(i, j);
			}
			else
			{
				greenChannelValue = getCompTimeAndGreenChannelInRedAndBluePixels(i, j);
				if (bayerImage.CurrentPixelIsRed(i,j))
				{
					redChannelValue = bayerImage.GetPixel(i, j);
					blueChannelValue = getBlueChannelInRedPixel(i, j);
				}
				else
				{
					blueChannelValue = bayerImage.GetPixel(i, j);
					redChannelValue = getRedChannelInBluePixel(i, j);
				}
			}
			
			filteredImage.SetPixel(correctedRowIndexForFilteredImage, correctedColIndexForFilteredImage, redChannelValue, RED);
			filteredImage.SetPixel(correctedRowIndexForFilteredImage, correctedColIndexForFilteredImage, greenChannelValue, GREEN);
			filteredImage.SetPixel(correctedRowIndexForFilteredImage, correctedColIndexForFilteredImage, blueChannelValue, BLUE);	
		}
	}
	
	return filteredImage;
}


/******* Closest Neighbor *******/ 

ClosestNeighbor :: ClosestNeighbor(BayerImage& aBayerImage) {
	this->bayerImage = aBayerImage;
	this->distanceFromPixelStartToBorder = 2;
	this->computationalTime = NOT_COMPUTED_FILTER;
}

ClosestNeighbor :: ~ClosestNeighbor(){}	

// EN PIXEL VERDE: Se toman los pixeles de la izquierda y arriba 
// EN OTROS: Se toman los pixeles de la izquierda y arriba a la izquierda

double ClosestNeighbor :: getRedChannelInGreenPixel(int i, int j){
	bool evenRow = i % 2 == 0;
	if (evenRow)
	{
		return bayerImage.GetPixel(i-1, j);
	}
	else
	{
		return bayerImage.GetPixel(i, j-1);
	}
}

double ClosestNeighbor :: getRedChannelInBluePixel(int i, int j){
	return bayerImage.GetPixel(i-1, j-1);
}

double ClosestNeighbor :: getGreenChannelInRedAndBluePixels(int i, int j){
	return bayerImage.GetPixel(i, j-1);
}

double ClosestNeighbor :: getBlueChannelInGreenPixel(int i, int j){
	bool evenRow = i % 2 == 0;
	if (evenRow)
	{
		return bayerImage.GetPixel(i, j-1);
	}
	else
	{
		return bayerImage.GetPixel(i-1, j);
	}
}

double ClosestNeighbor :: getBlueChannelInRedPixel(int i, int j)		{
	return bayerImage.GetPixel(i-1, j-1);
}

/******* Linear Interpolation Abstract Class *******/ 

// EN PIXEL VERDE: Se toman los pixeles de la izquierda y arriba 
// EN OTROS: Se toma la interpolación lineal horizontalmente y verticalmente (con los vecinos inmediatos)

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

double LinearInterpolation :: getRedChannelInGreenPixel(int i, int j){
	bool evenRow = i % 2 == 0;
	if (evenRow)
	{
		return interpolateVertical(i, j);
	}
	else
	{
		return interpolateHorizontal(i, j);
	}
}

double LinearInterpolation :: getBlueChannelInGreenPixel(int i, int j){
	bool evenRow = i % 2 == 0;
	if (evenRow)
	{
		return interpolateHorizontal(i, j);
	}
	else
	{
		return interpolateVertical(i, j);
	}
}

double LinearInterpolation :: getRedChannelInBluePixel(int i, int j){
	return interpolateCorners(i, j);
}

double LinearInterpolation :: getBlueChannelInRedPixel(int i, int j)		{
	return interpolateCorners(i, j);
}

/******* Bilinear Interpolation *******/ 

BilinearInterpolation :: BilinearInterpolation(BayerImage& aBayerImage) {
	this->bayerImage = aBayerImage;
	this->distanceFromPixelStartToBorder = 2;
	this->computationalTime = NOT_COMPUTED_FILTER;
}

BilinearInterpolation :: ~BilinearInterpolation(){}

double BilinearInterpolation :: getGreenChannelInRedAndBluePixels(int i, int j){
	return interpolateCross(i, j);
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
	
	for (; row < bayerImage.Height(); row = row + 2)
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

// Se utiliza SPLINES SÓLO en el CANAL VERDE

DirectionalInterpolation :: DirectionalInterpolation(BayerImage& aBayerImage) {
	this->bayerImage = aBayerImage;
	this->distanceFromPixelStartToBorder = 2;
	this->computationalTime = NOT_COMPUTED_FILTER;
}

DirectionalInterpolation :: ~DirectionalInterpolation(){}

double DirectionalInterpolation :: getGreenChannelInRedAndBluePixels(int i, int j){
	bool interpolateRow = horizontalGradient(i, j) < verticalGradient(i, j);
	vector<double> dataToInterpolate;
	vector<int> xValuesForData;
	if (interpolateRow)
	{
		dataToInterpolate = takeGreenFromRow(i, j, xValuesForData);
		return naturalCubicSplineInterpolation(j, xValuesForData, dataToInterpolate);			
	}
	else
	{
		dataToInterpolate = takeGreenFromCol(i, j, xValuesForData);
		return naturalCubicSplineInterpolation(i, xValuesForData, dataToInterpolate);			
	}
}

/******* Malvar He Cutler Interpolation *******/ 

double MalvarHeCutlerInterpolation :: gradientCorrection(int i, int j){
	return bayerImage.GetPixel(i, j) - 0.25 * ( bayerImage.GetPixel(i, j-2) + bayerImage.GetPixel(i, j+2) + bayerImage.GetPixel(i-2, j) + bayerImage.GetPixel(i+2, j) ); 
}


/******* Malvar He Cutler Filter *******/ 

// Se utiliza SPLINES SÓLO en el CANAL VERDE

MalvarHeCutler :: MalvarHeCutler(BayerImage& aBayerImage, double alpha) {
	this->bayerImage = aBayerImage;
	this->distanceFromPixelStartToBorder = 2;
	this->computationalTime = NOT_COMPUTED_FILTER;
	this->alpha = alpha;
}

MalvarHeCutler :: ~MalvarHeCutler(){}

double MalvarHeCutler :: getGreenChannelInRedAndBluePixels(int i, int j){
	return interpolateCross(i, j)  + alpha * gradientCorrection(i, j);
}
