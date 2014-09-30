#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include "ranking_algorithm.cpp"
#include "web_page.cpp"
#include "web_net.cpp"
//TODO DELETE WHAT WE ARE NOT USING

//TODO: REFACTOR INTO MULTIPLE FILES!

using namespace std;

#ifndef __PARSING__
#define __PARSING__

class ParsingAlgorithm {
    public:
        virtual WebNet ParseFile(const char* pathToFile) = 0;
        virtual void SaveRankTo(const char* savingFile, Rank aRank, AlgorithmType algorithmType){};
        virtual ~ParsingAlgorithm(){};
        
};

class TorontoParsing : public ParsingAlgorithm{
	public:
		WebNet ParseFile(const char* pathToFile);
        void SaveRankTo(const char* savingFile, Rank aRank, AlgorithmType algorithmType){};
		~TorontoParsing(){};
};

WebNet TorontoParsing ::  ParseFile(const char* pathToFile){
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
	
	WebNet net(amountOfNodes, amountOfEdges, webPages);
	return net;
}

class StanfordParsing : public ParsingAlgorithm{
	public:
		WebNet ParseFile(const char* pathToFile);
        void SaveRankTo(const char* savingFile, Rank aRank, AlgorithmType algorithmType){};
        ~StanfordParsing(){};
};

WebNet StanfordParsing :: ParseFile(const char* pathToFile){
	int amountOfNodes, amountOfEdges;
	list<WebPage*>* webPages = new list<WebPage*>();
	ifstream file;
	file.open(pathToFile);
	
	if (file.is_open()){
		list<WebPage*>* webPages = new list<WebPage*>();
		file >> amountOfNodes >> amountOfEdges;
		while( !file.eof() )
		{
			int fromNodeId;
			file >> fromNodeId ;
			int nodeId = fromNodeId;
			
			list<int>* listOfLinkedWebPagesIds = new list<int>();
			do {
				int toNodeId;
				file >> toNodeId;
				listOfLinkedWebPagesIds->push_back(toNodeId);
				fromNodeId = file.peek();
			} while(fromNodeId == nodeId);
			
			WebPage* node = new WebPage(nodeId, listOfLinkedWebPagesIds);
			webPages->push_back(node);
		}
		file.close();	
	}else{
		cout << "Unable to open input file" << endl;		
		exit(1);
	}
	WebNet net(amountOfNodes, amountOfEdges, webPages);
	return net;
}

#endif