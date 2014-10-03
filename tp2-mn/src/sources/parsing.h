#ifndef __PARSING__
#define __PARSING__

#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include "web_page.h"
#include "web_net.h"
//TODO DELETE WHAT WE ARE NOT USING

//TODO: REFACTOR INTO MULTIPLE FILES!

using namespace std;

enum InstanceType { STANFORD, TORONTO }; // Stanford = 0, Toronto = 1
enum AlgorithmType { PAGERANK, HITSALG, INDEG }; // PageRank = 0, HITSALG = 1, INDEG = 2

class ParsingAlgorithm {
    public:
        virtual WebNet* ParseFile(const char* pathToFile) = 0;
        void SaveRankTo(const char* pathToSavingFile, WebNet* aNet, AlgorithmType algorithmType);
        virtual ~ParsingAlgorithm(){};
        
};

class TorontoParsing : public ParsingAlgorithm{
	public:
		WebNet* ParseFile(const char* pathToFile);
        //void SaveRankTo(const char* pathToSavingFile, WebNet* aNet, AlgorithmType algorithmType);
		~TorontoParsing(){};
};

class StanfordParsing : public ParsingAlgorithm{
	public:
		WebNet* ParseFile(const char* pathToFile);
       // void SaveRankTo(const char* pathToSavingFile, WebNet* aNet, AlgorithmType algorithmType);
        ~StanfordParsing(){};
};

#endif
