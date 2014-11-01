#include "main.h"

using namespace std;

int main(int argc, char* argv[]) {	
	/*  --Pseudocodigo de pasos
	
	// ARGUMENTOS
	
	filename = GetFileName(first argument)
	qualityAlgorithm = GetQualityAlgorithm(second argument)
	filterType = GetFilterType(third argument)
	argumentsOfFilter = filterType.arguments(argc, argv[3:])


	// CREAR IMAGENES 
	
	bayerImage = imageFromFile(filename) // imageFromFile es un parser único, bayerImage es vector<vector>> (sabemos como es la forma)	
	filter = filterType.filter

	----- ACA MEDIR TIEMPO DE COMPUTO ---- --> ver desvio standard y promedio haciendo un for y corriendo muchas veces
	filteredImage = filter.image(bayerImage) //filteredImage es vector<vector<vector>>>
	----- ACA TERMINA MEDICION DE TIEMPO DE COMPUTO -----

	// SAVES
	
	qualityOfFilteredImage = qualityAlgorithm(filteredImage) // Cuantitativos: PSNR, SSIM
	saveImage(filteredImage)
	saveQualityOfFilteredImage(qualityOfFilteredImage)
	
	
	*/
	
	string filename(argv[1]);
	ParsingAlgorithm parser(filename);
	BayerImage bayerImage = parser.imageFromFile();
	bayerImage.viewImage();
	
	return 0;
}
