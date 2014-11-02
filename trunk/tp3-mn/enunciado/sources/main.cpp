#include "main.h"

using namespace std;

Filter* CreateFilterTypeFromParameter(FilterAlgorithmType filterAlgorithmType, BayerImage& bayerImage){
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
			MalvarHeCutler* MalvarHeCutlerFilter = new MalvarHeCutler(bayerImage);
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

QualityAlgorithm* CreateQualityAlgorithmFromParameter(QualityMeasurementType qualityMeasurementType){
	switch (qualityMeasurementType){
		case PSNR_Q:{
			PSNR* PSNRAlgorithm = new PSNR();
			return (QualityAlgorithm*)PSNRAlgorithm;
			break;
		}
		
		case SSIM_Q:{
			SSIM* SSIMAlgorithm = new SSIM();
			return (QualityAlgorithm*)SSIMAlgorithm;
			break;
		}
		
		default:{
			cout << "Haven't specified quality measurement type: impossible to qualify filtered image." << endl;
			exit(1);
			break;
		}
	}
}

int main(int argc, char* argv[]) {	
	/*  --Pseudocodigo de pasos
	
	// ARGUMENTOS
	
	filename = GetFileName(first argument)
	qualityAlgorithm = GetQualityAlgorithm(second argument)
	Filter = GetFilterType(third argument)
	argumentsOfFilter = Filter.arguments(argc, argv[3:])


	// CREAR IMAGENES 
	
	bayerImage = imageFromFile(filename) // imageFromFile es un parser único, bayerImage es vector<vector>> (sabemos como es la forma)	
	filter = Filter.filter

	----- ACA MEDIR TIEMPO DE COMPUTO ---- --> ver desvio standard y promedio haciendo un for y corriendo muchas veces
	filteredImage = filter.image(bayerImage) //filteredImage es vector<vector<vector>>>
	----- ACA TERMINA MEDICION DE TIEMPO DE COMPUTO -----

	// SAVES
	
	qualityOfFilteredImage = qualityAlgorithm(filteredImage) // Cuantitativos: PSNR, SSIM
	saveImage(filteredImage)
	saveQualityOfFilteredImage(qualityOfFilteredImage)
	
	
	*/
	
	string filename(argv[1]);
	int qualityMeasurementType = atoi(argv[2]);
	int filterType = atoi(argv[3]);
	
	ParsingAlgorithm parser(filename);
	BayerImage bayerImage = parser.ImageFromFile();
	
	Filter* filter = CreateFilterTypeFromParameter((FilterAlgorithmType)filterType, bayerImage);
	QualityAlgorithm* qualityAlgorithm = CreateQualityAlgorithmFromParameter((QualityMeasurementType)qualityMeasurementType);
	ColorImage filteredImage = filter->FilterImage();
	parser.SaveImage(filteredImage);
	
	delete filter;
	delete qualityAlgorithm;
	return 0;
}
