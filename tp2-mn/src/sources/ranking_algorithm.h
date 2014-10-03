#ifndef __RANKING_ALGORITHMS__
#define __RANKING_ALGORITHMS__

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
#include "matrix.h"

using namespace std;

void scaleBy(double aScalingFactor, vector<double>& aVector);

vector<double> createRandomVectorOfSize(int n);

double calculateNorm2(vector<double>& aVector);

void normalizeVector(vector<double>& aVector);

double sumElements(vector<double>& aVector);

void addConstantToEachElement(double aConstant, vector<double>& aVector);

class RankingAlgorithm { //should be created with a PerformanceAnalyzer object
    public:
        virtual void RankPage(WebNet* net, int amountOfIterations) = 0; //i think this won't be void but a orderd list of ranked pages.
        virtual ~RankingAlgorithm(){};
        
};

class PageRank : public RankingAlgorithm {
    public:
        PageRank(double teletransportingProbability) : _teletransporting(teletransportingProbability) {}
        ~PageRank();
    
        void RankPage(WebNet* net, int amountOfIterations);
        
    private:
        CRSMatrix createAdjacencyMatrix(WebNet* net);
        void updateNetWithRanks(vector<double> eigenvector, WebNet* net);
        
        double _teletransporting;
};

class HITS : public RankingAlgorithm {
    public:
        void RankPage(WebNet* net, int amountOfIterations);
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
		
		AuthorityHubWeightVectors Iterate(CRSMatrix& adjacencyMatrix, CRSMatrix& transposedAdjacencyMatrix, int amountOfIterations);
};

class InDegree : public RankingAlgorithm {
    public:
        void RankPage(WebNet* net, int amountOfIterations);
        ~InDegree(){};
};

#endif
