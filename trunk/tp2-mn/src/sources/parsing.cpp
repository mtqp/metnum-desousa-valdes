#include "parsing.h"

using namespace std;

void ParsingAlgorithm :: SaveRankTo(const char* pathToSavingFile, WebNet* aNet, AlgorithmType algorithmType){
	ofstream file;
	file.open(pathToSavingFile);
	if (file.is_open()){
		list<WebPage*>* webPages = aNet->webPages();
		if (algorithmType != HITSALG){
			list<WebPage*>::iterator itNetPages = webPages->begin();
			
			for (int webPageId = 1; webPageId <= aNet->amountOfNodes(); webPageId++){
				//~ if (itNetPages != webPages->end() && webPageId == (*itNetPages)->pageId()){
					PageRankInDegreeRank* rankingOfPage = (PageRankInDegreeRank*)((*itNetPages)->ranking());
					file << *rankingOfPage;
					itNetPages++;
				//~ } else {
					//~ file << "0" << endl;
					//~ cout << webPageId << endl;
				//~ }
			}
		} else {
			list<WebPage*>::iterator itNetPages = webPages->begin();
			
			for (int webPageId = 1; webPageId <= aNet->amountOfNodes(); webPageId++){
				//~ if (itNetPages != webPages->end() && webPageId == (*itNetPages)->pageId()){
					HITSRank* rankingOfPage = (HITSRank*)((*itNetPages)->ranking());
					file << rankingOfPage->authorityRank() << endl;
					itNetPages++;
				//~ } else {
					//~ file << "0" << endl;
					//~ cout << webPageId << endl;
				//~ }
			}
			
			itNetPages = webPages->begin();
			for (int webPageId = 1; webPageId <= aNet->amountOfNodes(); webPageId++){
				//~ if (itNetPages != webPages->end() && webPageId == (*itNetPages)->pageId()){
					HITSRank* rankingOfPage = (HITSRank*)((*itNetPages)->ranking());
					file << rankingOfPage->hubRank() << endl;
					itNetPages++;
				//~ } else {
					//~ file << "0" << endl;
					//~ cout << webPageId << endl;
				//~ }
			}
		}
					
	} else{
		cout << "Unable to open output file" << endl;		
		exit(1);
	}
}

WebNet* TorontoParsing :: ParseFile(const char* pathToFile){
	int amountOfNodes, amountOfEdges = 0;
	list<WebPage*>* webPages = new list<WebPage*>();
	ifstream file;
	file.open(pathToFile);
	
	if (file.is_open()){
		int fromNodeId;
		while( !file.eof() )
		{
			file >> fromNodeId ;
			int nodeId = fromNodeId;
			file.ignore(); 				// ignora el :
			int toNodeId;
			file >> toNodeId; 
			
			list<int>* listOfLinkedWebPagesIds = new list<int>();
			while(toNodeId != -1){
				listOfLinkedWebPagesIds->push_back(toNodeId);
				file >> toNodeId;
				if (toNodeId != -1){
					amountOfEdges++;
				}
			}	
			WebPage* node = new WebPage(nodeId, listOfLinkedWebPagesIds);  	//ver
			if(!listOfLinkedWebPagesIds->empty()){
				webPages->push_back(node);
			}
		}
		amountOfNodes = fromNodeId;
		file.close();	
	} else{
		cout << "Unable to open input file" << endl;		
		exit(1);
	}
	
	WebNet* net = new WebNet(amountOfNodes, amountOfEdges, webPages);
	return net;
}

WebNet* StanfordParsing :: ParseFile(const char* pathToFile){
	int amountOfNodes, amountOfEdges;
	list<WebPage*>* webPages = new list<WebPage*>();
	ifstream file;
	file.open(pathToFile);
	
	if (file.is_open()){
		
		file >> amountOfNodes >> amountOfEdges;
		
		for (int i = 1; i <= amountOfNodes; i++){
			list<int>* listOfLinkedWebPagesIds = new list<int>();
			WebPage* node = new WebPage(i, listOfLinkedWebPagesIds);
			webPages->push_back(node);
		}
		
		int fromNodeId;
		file >> fromNodeId ;
		while( !file.eof() )
		{
			int nodeId = fromNodeId;			
			while(fromNodeId == nodeId && !file.eof()) {
				list<WebPage*>::iterator webPagesIt = webPages->begin();
				advance(webPagesIt,nodeId-1);
				int toNodeId;
				file >> toNodeId;
				(*webPagesIt)->addToLinkedIdList(toNodeId);
				file >> fromNodeId;
			}
		}
		file.close();	
	}else{
		cout << "Unable to open input file" << endl;		
		exit(1);
	}
	WebNet* net = new WebNet(amountOfNodes, amountOfEdges, webPages);
	return net;
}
