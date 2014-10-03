#include "main.h"

//TODO DELETE WHAT WE ARE NOT USING

//TODO: REFACTOR INTO MULTIPLE FILES!

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
	AlgorithmType algorithmToRun = (AlgorithmType)atoi(argv[1]);
	double teletransportingValue = atof(argv[2]);
	InstanceType instanceType = (InstanceType)atoi(argv[3]);
	char* webDefinitionPathFile = argv[4];
	double toleranceValue = atof(argv[5]);
	
	char* savingFile;// = "ranking.out";
	if (argc == 7)
		savingFile = argv[6];
	
	ParsingAlgorithm* parsingAlgorithm = CreateParsingAlgorithmFromParameter(instanceType);
	WebNet* net = parsingAlgorithm->ParseFile(webDefinitionPathFile);	
	
	int amountOfIterations = 50;
	//int amountOfResultsToShow = 10;
	RankingAlgorithm* rankingAlgorithm = CreateRankingAlgorithmFromParameter(algorithmToRun);
	rankingAlgorithm->RankPage(net, amountOfIterations);
	
	parsingAlgorithm->SaveRankTo(savingFile, net, algorithmToRun);
	
	delete parsingAlgorithm;
	delete rankingAlgorithm;
	delete net;
	return 0;
}