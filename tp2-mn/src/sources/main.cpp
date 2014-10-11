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

RankingAlgorithm* CreateRankingAlgorithmFromParameter(AlgorithmType algorithmType, double teletransportingValue, double toleranceValue){
	switch (algorithmType){
		case PAGERANK:{
			PageRank* pageRankAlgorithm = new PageRank(teletransportingValue,toleranceValue);
			return (RankingAlgorithm*)pageRankAlgorithm;
			break;
		}
		
		case HITSALG:{
			HITS* HITSAlgorithm = new HITS(toleranceValue);
			return (RankingAlgorithm*)HITSAlgorithm;
			break;
		}
		
		case INDEG:{
			InDegree* InDegAlgorithm = new InDegree();
			return (RankingAlgorithm*)InDegAlgorithm;
			break;
		}
		
		default:{
			PageRank* pageRankAlgorithm = new PageRank(teletransportingValue,toleranceValue);
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
		inputFile >> teletransportingValue;
		inputFile >> instanceType;
		inputFile >> webDefinitionPathFile;
		inputFile >> toleranceValue;
		inputFile.close();	
	} else{
		cout << "Unable to open input (.in) file" << endl;		
		exit(1);
	}

	string savingFile(argv[2]);
	
	ParsingAlgorithm* parsingAlgorithm = CreateParsingAlgorithmFromParameter((InstanceType)instanceType);
    
	WebNet* net = parsingAlgorithm->ParseFile(webDefinitionPathFile);	

	RankingAlgorithm* rankingAlgorithm = CreateRankingAlgorithmFromParameter((AlgorithmType)algorithmToRun, teletransportingValue, toleranceValue);
	
    rankingAlgorithm->RankPage(net);
    
	parsingAlgorithm->SaveRankTo(savingFile.c_str(), net, (AlgorithmType)algorithmToRun);

    
	delete parsingAlgorithm;
	delete rankingAlgorithm;
	delete net;
	return 0;
}
