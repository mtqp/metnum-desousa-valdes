#include "parsing.h"

using namespace std;

void ParsingAlgorithm :: SaveQualityOfFilteredImage(){
	ofstream file;
	file.open(qualityFilename().c_str());
	if (file.is_open()){
		cout << "Falta implementar" << endl;					
	} else{
		cout << "Unable to open output file" << endl;		
		exit(1);
	}
}

void ParsingAlgorithm :: saveImageChannel(string filename, const vector<vector<double> >& valuesToSave){
	ofstream file;
	file.open(filename.c_str());
	if (file.is_open()){
		for (int i = 0; i < (int)valuesToSave.size(); i++)
		{
			for (int j = 0; j < (int)valuesToSave[i].size(); j++)
			{
				file << valuesToSave[i][j] << "\t";
			}
			file << endl;
			
		}
		cout << string(filename) + string(" channel image file saved!") << endl;
		file.close();
	} else{
		cout << "Unable to open output file" << endl;		
		exit(1);
	}
	
}

void ParsingAlgorithm :: SaveImage(ColorImage anImage){
	ofstream file;
	string r("_r");
	string g("_g");
	string b("_b");
	string filename = savingImageFilename(); 
	
	saveImageChannel(filename + r, anImage.ChannelR());
	saveImageChannel(filename + g, anImage.ChannelG());
	saveImageChannel(filename + b, anImage.ChannelB());
}

void ParsingAlgorithm :: SaveComputationalTime(){
	ofstream file;
	file.open(computationalTimeFilename().c_str());
	if (file.is_open()){
		cout << "Falta implementar" << endl;
		file.close();
	} else{
		cout << "Unable to open output file" << endl;		
		exit(1);
	}
}


BayerImage ParsingAlgorithm :: ImageFromFile(){
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
				bayerImage.SetPixel(i, j, currentPixel);
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

