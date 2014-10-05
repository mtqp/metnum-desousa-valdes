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

typedef list<int>::iterator list_iterator;

void scaleBy(double aScalingFactor, vector<double>& aVector);

vector<double> createRandomVectorOfSize(int n, unsigned int srand_seed);

double calculateNorm2(vector<double>& aVector);

void normalizeVector(vector<double>& aVector);

double sumElements(vector<double>& aVector);

void addConstantToEachElement(double aConstant, vector<double>& aVector);

vector<double> substractVectors(vector<double>& v1, vector<double>& v2);

vector<double> sumVectors(vector<double>& v1, vector<double>& v2);

class RankingAlgorithm {
    public:
        virtual void RankPage(WebNet* net, int amountOfIterations) = 0;
        virtual ~RankingAlgorithm(){};
        
};

class PageRank : public RankingAlgorithm {
    public:
        PageRank(double teletransportingProbability, double cutTolerance) : _teletransporting(teletransportingProbability), _cutTolerance(cutTolerance) {}
        ~PageRank();
    
        void RankPage(WebNet* net, int amountOfIterations);
        
    private:
		void prepareBuildersForPDMatrices(WebNet* net, CRSBuilder& builderP, CRSBuilder& builderD);
		double differenceBetweenSolutions(vector<double>& x, vector<double>& old_x);
        void updateNetWithRanks(vector<double> eigenvector, WebNet* net);
        
        double _teletransporting;
        double _cutTolerance;
};

class HITS : public RankingAlgorithm {
    public:
        void RankPage(WebNet* net, int amountOfIterations);
        ~HITS(){};
    private:
		class AuthorityHubWeightVectors {
			public:
				AuthorityHubWeightVectors(vector<double>& authWVector, vector<double>& hubWVector) : authorityWeightVector(authWVector), hubWeightVector(hubWVector) {}
				~AuthorityHubWeightVectors(){};
				
				vector<double> authorityWeightVector;
				vector<double> hubWeightVector;
				
				void normalizeVectors(){
					normalizeVector(authorityWeightVector);
					normalizeVector(hubWeightVector);
				}
		};
		
		AuthorityHubWeightVectors Iterate(CRSMatrix& adjacencyMatrix, CRSMatrix& transposedAdjacencyMatrix, int amountOfIterations);
		
};

class InDegree : public RankingAlgorithm {
    public:
        void RankPage(WebNet* net, int amountOfIterations);
        ~InDegree(){};
};

#endif
