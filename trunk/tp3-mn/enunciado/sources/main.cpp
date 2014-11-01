#include "main.h"

using namespace std;

Filter* CreateFilterTypeFromParameter(FilterAlgorithmType filterAlgorithmType){
	switch (filterAlgorithmType){
		case CLOSEST_NEIGHBOR:{
			ClosestNeighbor* closestNeighborFilter = new ClosestNeighbor();
			return (Filter*)closestNeighborFilter;
			break;
		}
		
		case BILINEAR_INTERPOLATION:{
			BilinearInterpolation* bilinearInterpolationFilter = new BilinearInterpolation();
			return (Filter*)bilinearInterpolationFilter;
			break;
		}
		
		case DIRECTIONAL_INTERPOLATION:{
			DirectionalInterpolation* directionalInterpolationFilter = new DirectionalInterpolation();
			return (Filter*)directionalInterpolationFilter;
			break;
		}
		
		case MALVAR_HE_CUTLER:{
			MalvarHeCutler* MalvarHeCutlerFilter = new MalvarHeCutler();
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
	BayerImage bayerImage = parser.imageFromFile();
	
	Filter* filter = CreateFilterTypeFromParameter((FilterAlgorithmType)filterType);
	QualityAlgorithm* qualityAlgorithm = CreateQualityAlgorithmFromParameter((QualityMeasurementType)qualityMeasurementType);
	
	delete filter;
	delete qualityAlgorithm;
	return 0;
}
