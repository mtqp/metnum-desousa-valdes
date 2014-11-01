#include "parsing.h"

using namespace std;

void ParsingAlgorithm :: saveQualityOfFilteredImage(){
	ofstream file;
	file.open(qualityFilename());
	if (file.is_open()){
		cout << "Falta implementar" << endl;					
	} else{
		cout << "Unable to open output file" << endl;		
		exit(1);
	}
}

void ParsingAlgorithm :: saveImage(ColorImage anImage){
	ofstream file;
	file.open(savingImageFilename());
	if (file.is_open()){
		cout << "Falta implementar" << endl;
	} else{
		cout << "Unable to open output file" << endl;		
		exit(1);
	}
}

void ParsingAlgorithm :: saveComputationalTime(){
	ofstream file;
	file.open(computationalTimeFilename());
	if (file.is_open()){
		cout << "Falta implementar" << endl;
					
	} else{
		cout << "Unable to open output file" << endl;		
		exit(1);
	}
}


BayerImage ParsingAlgorithm :: imageFromFile(){
	int imageWidth, imageHeight;
	ifstream file;
	file.open(filename);
	
	BayerImage bayerImage;

	if (file.is_open()){
		file >> imageWidth >> imageHeight;
		bayerImage = BayerImage(imageWidth, imageHeight);

		for (int i = 0; i < imageHeight; i++)
		{
			for (int j = 0; j < imageWidth; j++)
			{
				double currentPixel;
				file >> currentPixel;
				bayerImage.setPixel(i, j, currentPixel);
			}
		}
		file.close();	
	} else{
		cout << "Unable to open input file" << endl;		
		exit(1);
	}
	
	return bayerImage;
}

string ParsingAlgorithm :: computationalTimeFilename(){
	string computationalTimeAppendix;
	return filename + computationalTimeAppendix;
}

string ParsingAlgorithm :: savingImageFilename(){
	string savingImageAppendix;
	return filename + savingImageAppendix;
}

string ParsingAlgorithm :: qualityFilename(){
	string qualityAppendix;
	return filename + qualityAppendix;
}      

