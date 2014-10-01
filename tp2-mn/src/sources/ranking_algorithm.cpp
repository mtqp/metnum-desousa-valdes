#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include "web_page.cpp"
#include "web_net.cpp"
#include "matrix.cpp"

using namespace std;

#ifndef __RANKING_ALGORITHMS__
#define __RANKING_ALGORITHMS__

class RankingAlgorithm { //should be created with a PerformanceAnalyzer object
    public:
        virtual void RankPage(WebNet* net, int amountOfIterations, int amountOfResultsToShow) = 0; //i think this won't be void but a orderd list of ranked pages.
        ~RankingAlgorithm(){};
        
};


class PageRank : public RankingAlgorithm {
    public:
        void RankPage(WebNet* net, int amountOfIterations, int amountOfResultsToShow);
        ~PageRank(){};
};

void PageRank :: RankPage(WebNet* net, int amountOfIterations, int amountOfResultsToShow){
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

double calculateNorm2(vector<double>& aVector){
	double squaresSum = 0;
	for (int i = 0; i < (int)aVector.size(); i++)
	{
		squaresSum += aVector[i]*aVector[i];
	}
	
	return sqrt(squaresSum);
}

void normalizeVector(vector<double>& aVector){
	double aVectorNorm2 = calculateNorm2(aVector);
	
	for (int i = 0; i < (int)aVector.size(); i++)
	{
		aVector[i] = aVector[i]/aVectorNorm2;
	}
}

//bool orderMostImportantFirst (int i,int j) { return (i>j); }

class HITS : public RankingAlgorithm {
    public:
        void RankPage(WebNet* net, int amountOfIterations, int amountOfResultsToShow);
        ~HITS(){};
    private:
		class AuthorityHubWeightVectors {
			public:
				AuthorityHubWeightVectors(vector<double>& authWVector, vector<double>& hubWVector) : authorityWeightVector(authWVector), hubWeightVector(hubWVector) {}
				~AuthorityHubWeightVectors(){
					/*
					delete _authorityWeightVector;
					delete _hubWeightVector;
					*/
				};
				
				vector<double> authorityWeightVector;
				vector<double> hubWeightVector;
				
				void normalizeVectors(){
					normalizeVector(authorityWeightVector);
					normalizeVector(hubWeightVector);
				}
				
				/*void sortVectorsMostImportantFirst(){
					sort(authorityWeightVector.begin(), authorityWeightVector.end(), orderMostImportantFirst);
					sort(hubWeightVector.begin(), hubWeightVector.end(), orderMostImportantFirst);
				}*/
		};
		
		AuthorityHubWeightVectors Iterate(CRSMatrix adjacencyMatrix, int amountOfIterations){
			vector<double> authorityWeightVector = vector<double>(adjacencyMatrix.amountOfColumns(), 1);
			vector<double> hubWeightVector = vector<double>(adjacencyMatrix.amountOfColumns(), 1);
			AuthorityHubWeightVectors authorityHubWeightVectors(authorityWeightVector, hubWeightVector);
			
			for (int i = 1; i <= amountOfIterations; i++){
				authorityHubWeightVectors.authorityWeightVector = adjacencyMatrix.Multiply(authorityHubWeightVectors.hubWeightVector); // Falta hacer la traspuesta de adjacencyMatrix!!
				authorityHubWeightVectors.hubWeightVector = adjacencyMatrix.Multiply(authorityHubWeightVectors.authorityWeightVector);
				authorityHubWeightVectors.normalizeVectors();
			}
			
			return authorityHubWeightVectors;
		}
};

void HITS :: RankPage(WebNet* net, int amountOfIterations, int amountOfResultsToShow){
	
	/** Create adjacency matrix **/
	CRSBuilder builder;
	for (list<WebPage*>::iterator itNetPages = (net->webPages())->begin(); itNetPages != (net->webPages())->end(); ++itNetPages){
		for (list<int>::iterator itIdLinkedPages = ((*itNetPages)->listOfLinkedWebPagesIds())->begin(); itIdLinkedPages != ((*itNetPages)->listOfLinkedWebPagesIds())->end(); ++itIdLinkedPages){
			builder.AddElementAt((*itNetPages)->pageId(), *itIdLinkedPages, 1.0);
		}
	}
    
    CRSMatrix adjacencyMatrix = builder.Build(net->amountOfNodes(),net->amountOfNodes());
    
    /** Find weights (hub & authority) **/
	AuthorityHubWeightVectors authorityHubWeightVectors = Iterate(adjacencyMatrix, amountOfIterations);
	
	
	/** Rank WebPages **/ 
	for (list<WebPage*>::iterator itNetPages = net->webPages()->begin(); itNetPages != net->webPages()->end(); ++itNetPages){
		HITSRank* webPageRanking = new HITSRank(authorityHubWeightVectors.authorityWeightVector[(*itNetPages)->pageId()], authorityHubWeightVectors.hubWeightVector[(*itNetPages)->pageId()]);
		(*itNetPages)->rankWebPage( (Rank*)webPageRanking );
	}
	
	/// Show results
	/*authorityHubWeightVectors.sortVectorsMostImportantFirst();
	for (int i = 0; i < amountOfResultsToShow; i++){
		
	}*/
}

class InDegree : public RankingAlgorithm {
    public:
        void RankPage(WebNet* net, int amountOfIterations, int amountOfResultsToShow){};
        ~InDegree(){};
};

#endif
