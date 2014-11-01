#include "parsing.h"

using namespace std;

void ParsingAlgorithm :: saveQualityOfFilteredImage(){
	ofstream file;
	file.open(qualityFilename().c_str());
	if (file.is_open()){
		cout << "Falta implementar" << endl;					
	} else{
		cout << "Unable to open output file" << endl;		
		exit(1);
	}
}

void ParsingAlgorithm :: saveImage(ColorImage anImage){
	ofstream file;
	string r("_r");
	string g("_g");
	string b("_b");
	
	// Channel R
	file.open((savingImageFilename() + r).c_str());
	if (file.is_open()){
		cout << "Falta implementar" << endl;
	} else{
		cout << "Unable to open output file" << endl;		
		exit(1);
	}
	
	// Channel G
	file.open((savingImageFilename() + g).c_str());
	if (file.is_open()){
		cout << "Falta implementar" << endl;
	} else{
		cout << "Unable to open output file" << endl;		
		exit(1);
	}
	
	// Channel B
	file.open((savingImageFilename() + b).c_str());
	if (file.is_open()){
		cout << "Falta implementar" << endl;
	} else{
		cout << "Unable to open output file" << endl;		
		exit(1);
	}
}

void ParsingAlgorithm :: saveComputationalTime(){
	ofstream file;
	file.open(computationalTimeFilename().c_str());
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
	file.open(filename.c_str());
	
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
	string computationalTimeAppendix("_ctime");
	return filename + computationalTimeAppendix;
}

string ParsingAlgorithm :: savingImageFilename(){
	string savingImageAppendix("_out");
	return filename + savingImageAppendix;
}

string ParsingAlgorithm :: qualityFilename(){
	string qualityAppendix("_quality");
	return filename + qualityAppendix;
}      

