#include "main.h"

using namespace std;

Filter* CreateFilterTypeFromParameter(FilterAlgorithmType filterAlgorithmType, BayerImage& bayerImage, double alpha){
	switch (filterAlgorithmType){
		case CLOSEST_NEIGHBOR:{
			ClosestNeighbor* closestNeighborFilter = new ClosestNeighbor(bayerImage);
			return (Filter*)closestNeighborFilter;
			break;
		}
		
		case BILINEAR_INTERPOLATION:{
			BilinearInterpolation* bilinearInterpolationFilter = new BilinearInterpolation(bayerImage);
			return (Filter*)bilinearInterpolationFilter;
			break;
		}
		
		case DIRECTIONAL_INTERPOLATION:{
			DirectionalInterpolation* directionalInterpolationFilter = new DirectionalInterpolation(bayerImage);
			return (Filter*)directionalInterpolationFilter;
			break;
		}
		
		case MALVAR_HE_CUTLER:{
			MalvarHeCutler* MalvarHeCutlerFilter = new MalvarHeCutler(bayerImage, alpha);
			return (Filter*)MalvarHeCutlerFilter;
			break;
		}
		
		default:{
			cout << "Haven't specified filter type: impossible to filter image." << endl;
			exit(1);
			break;
		}
	}
}

int main(int argc, char* argv[]) {	
	/*  --Pseudocodigo de pasos
	
	// ARGUMENTOS
	
	filenameImageMosaic = GetFileName(first argument)
	Filter = GetFilterType(third argument)
	argumentsOfFilter = Filter.arguments(argc, argv[3:])


	// CREAR IMAGENES 
	
	bayerImage = imageFromFile(filenameImageMosaic) // imageFromFile es un parser único, bayerImage es vector<vector>> (sabemos como es la forma)	
	filter = Filter.filter

	----- ACA MEDIR TIEMPO DE COMPUTO ---- --> ver desvio standard y promedio haciendo un for y corriendo muchas veces
	filteredImage = filter.image(bayerImage) //filteredImage es vector<vector<vector>>>
	----- ACA TERMINA MEDICION DE TIEMPO DE COMPUTO -----

	// SAVES
	
	saveImage(filteredImage)
	
	// QUALITY --> se hace en matlab
	
	*/
	
	string filenameImageMosaic(argv[1]);
	FilterAlgorithmType filterType = (FilterAlgorithmType) atoi(argv[2]);
	
	// Malvar He Cutler parameter
	double alpha;
	if (filterType == MALVAR_HE_CUTLER) alpha = atof(argv[3]);
	
	ParsingAlgorithm parser(filenameImageMosaic);
	BayerImage bayerImage = parser.ImageFromFile();
	
	Filter* filter = CreateFilterTypeFromParameter(filterType, bayerImage, alpha);
	ColorImage filteredImage = filter->FilterImage();
	parser.SaveImage(filteredImage);
	cout << "Green channel computational time clocks: " << filter->ComputationalTime() << endl;
	
	delete filter;
	return 0;
}
