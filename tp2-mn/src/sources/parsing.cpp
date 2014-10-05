#include "parsing.h"

using namespace std;

void ParsingAlgorithm :: SaveRankTo(const char* pathToSavingFile, WebNet* aNet, AlgorithmType algorithmType){
	ofstream file;
	file.open(pathToSavingFile);
	if (file.is_open()){
		list<WebPage*>* webPages = aNet->webPages();
		if (algorithmType != HITSALG){
			list<WebPage*>::iterator itNetPages = webPages->begin();
			//cout << "List size: " << webPages->size() << endl;
            //cout << "Amount of nodes: " << aNet->amountOfNodes() << endl;
            
			for (int webPageId = 1; webPageId <= aNet->amountOfNodes(); webPageId++){
                PageRankInDegreeRank* rankingOfPage = (PageRankInDegreeRank*)((*itNetPages)->ranking());
				file << *rankingOfPage;
				itNetPages++;
			}
		} else {
			list<WebPage*>::iterator itNetPages = webPages->begin();
			
			for (int webPageId = 1; webPageId <= aNet->amountOfNodes(); webPageId++){
				HITSRank* rankingOfPage = (HITSRank*)((*itNetPages)->ranking());
				file << rankingOfPage->authorityRank() << endl;
				itNetPages++;
			}
			
			itNetPages = webPages->begin();
			for (int webPageId = 1; webPageId <= aNet->amountOfNodes(); webPageId++){
				HITSRank* rankingOfPage = (HITSRank*)((*itNetPages)->ranking());
				file << rankingOfPage->hubRank() << endl;
				itNetPages++;
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
			webPages->push_back(node);
		}
		amountOfNodes = fromNodeId + 1; //'cause it's zero based
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
		//ignoro lineas
		file.ignore(1000,'\n');
		file.ignore(1000,'\n');
		
		//ignoro hasta : 
		
		file.ignore(200, ':');
		file >> amountOfNodes;
		
		//ignoro hasta : 
		file.ignore(200, ':');
		file >> amountOfEdges;
		
		// ignoro lineas
		file.ignore(1000,'\n');
		file.ignore(1000,'\n');
		
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
