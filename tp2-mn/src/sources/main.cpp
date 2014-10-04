#include "main.h"

using namespace std;

ParsingAlgorithm* CreateParsingAlgorithmFromParameter(InstanceType instanceType){
	switch (instanceType){
		case STANFORD:{
			StanfordParsing* stanfordParsingAlgorithm = new StanfordParsing();
			return (ParsingAlgorithm*)stanfordParsingAlgorithm;
			break;
		}
		
		case TORONTO:{
			TorontoParsing* torontoParsingAlgorithm = new TorontoParsing();
			return (ParsingAlgorithm*)torontoParsingAlgorithm;
			break;
		}
		
		default:{
			cout << "Haven't specified instance type: impossible to parse input file." << endl;
			exit(1);
			break;
		}
	}
}

RankingAlgorithm* CreateRankingAlgorithmFromParameter(AlgorithmType algorithmType){
	switch (algorithmType){
		case PAGERANK:{
			PageRank* pageRankAlgorithm = new PageRank(0.85); //TODO: REMEMBER TO REFACTOR THIS! THIS IS RETRIEVED FROM MAIN PARAMETERS!!!
			return (RankingAlgorithm*)pageRankAlgorithm;
			break;
		}
		
		case HITSALG:{
			HITS* HITSAlgorithm = new HITS();
			return (RankingAlgorithm*)HITSAlgorithm;
			break;
		}
		
		case INDEG:{
			InDegree* InDegAlgorithm = new InDegree();
			return (RankingAlgorithm*)InDegAlgorithm;
			break;
		}
		
		default:{
			PageRank* pageRankAlgorithm = new PageRank(0.85); //TODO: REMEMBER TO REFACTOR THIS! THIS IS RETRIEVED FROM MAIN PARAMETERS!!!
			return (RankingAlgorithm*)pageRankAlgorithm;
			break;
		}
	}
}


int main(int argc, char* argv[]) {	
/*  --Pseudocodigo de pasos
	TeletransportingValue c = GetTeletransportingValue(second argument)
	double tolerance = RetrieveToleranceFrom(fifth argument)

	RankingAlgorithm rankingAlgorithm = CreateRankingAlgorithmFromParameter(first argument) //PageRangk || HITS || INDEG
	ParsingAlgorithm parsingAlgorithm = CreateParsingAlgorithmFromParameter(third argument)

	list<WebPage> pages = parsingAlgorithm.ParseFile(pathRetrievedFromFourthArgument)
	
	Rank aRank = rankingAlgorithm.rank(pages);
	
	parsingAlgorithm.SaveRankTo(savingFile, aRank);*/
	ifstream inputFile;
	
	inputFile.open(argv[1]);
	
	int algorithmToRun;
	double teletransportingValue;
	int instanceType;
	char webDefinitionPathFile[256];
	double toleranceValue;
	
	if (inputFile.is_open()){
		inputFile >> algorithmToRun;
		cout << "algoRun: " << algorithmToRun << endl;
		inputFile >> teletransportingValue;
				cout << "teletransportingValue: " << teletransportingValue << endl;
		inputFile >> instanceType;
				cout << "instanceType: " << instanceType << endl;
		inputFile >> webDefinitionPathFile;
				cout << "webDefinitionPathFile: " << webDefinitionPathFile << endl;
		inputFile >> toleranceValue;
				cout << "toleranceValue: " << toleranceValue << endl;
		inputFile.close();	
	} else{
		cout << "Unable to open input file" << endl;		
		exit(1);
	}
	
	string savingFile(argv[2]);
	
	
	ParsingAlgorithm* parsingAlgorithm = CreateParsingAlgorithmFromParameter((InstanceType)instanceType);
	WebNet* net = parsingAlgorithm->ParseFile(webDefinitionPathFile);	
	
	int amountOfIterations = 50;
	//int amountOfResultsToShow = 10;
	RankingAlgorithm* rankingAlgorithm = CreateRankingAlgorithmFromParameter((AlgorithmType)algorithmToRun);
	rankingAlgorithm->RankPage(net, amountOfIterations);
	
	parsingAlgorithm->SaveRankTo(savingFile.c_str(), net, (AlgorithmType)algorithmToRun);
	
	delete parsingAlgorithm;
	delete rankingAlgorithm;
	delete net;
	return 0;
}
