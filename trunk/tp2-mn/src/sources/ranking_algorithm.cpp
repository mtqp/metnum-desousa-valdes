#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include "web_page.cpp"

using namespace std;

#ifndef __RANKING_ALGORITHMS__
#define __RANKING_ALGORITHMS__

enum InstanceType { STANFORD, TORONTO }; // Stanford = 0, Toronto = 1
enum AlgorithmType { PAGERANK, HITSALG, INDEG }; // PageRank = 0, HITSALG = 1, INDEG = 2

class RankingAlgorithm { //should be created with a PerformanceAnalyzer object
    public:
        virtual void Rank(vector<WebPage> pages){} //i think this won't be void but a orderd list of ranked pages.
        ~RankingAlgorithm(){};
        
};


class PageRank : public RankingAlgorithm {
    public:
        void Rank(vector<WebPage> pages);
        ~PageRank(){};
};

void PageRank :: Rank(vector<WebPage> pages){
    /*
    Matrix matrix = CreateMatrixWith(pages);
    foreach page in pages
    {
    	if(no se autorankea la page)
	     	matrix.AddElementA(page.I, page.J, page.value);
    }
    
       
    while(!decidedToStop)
    {
        perform cA * ((1-c)/n)TODOS_UNOS x= x
    }
    
    return OrderPages() might need the matrix and the pages
    */
}

class HITS : public RankingAlgorithm {
    public:
        void Rank(vector<WebPage> pages){};
        ~HITS(){};
};

class InDegree : public RankingAlgorithm {
    public:
        void Rank(vector<WebPage> pages){};
        ~InDegree(){};
};

#endif