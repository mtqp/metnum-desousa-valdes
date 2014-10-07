#ifndef __RANKING_ALGORITHMS__
#define __RANKING_ALGORITHMS__

#include <iostream>
#include <fstream>
#include <sstream>
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

double manhattanNorm(vector<double>& aVector);

void addConstantToEachElement(double aConstant, vector<double>& aVector);

vector<double> substractVectors(vector<double>& v1, vector<double>& v2);

vector<double> sumVectors(vector<double>& v1, vector<double>& v2);

double norm2DifferenceBetweenSolutions(vector<double>& x, vector<double>& old_x);

class RankingAlgorithm {
    public:
        virtual void RankPage(WebNet* net) = 0;
        virtual ~RankingAlgorithm(){};
        
};

class PageRank : public RankingAlgorithm {
    public:
        PageRank(double teletransportingProbability, double cutTolerance) : _teletransporting(teletransportingProbability), _cutTolerance(cutTolerance) {}
        ~PageRank();
    
        void RankPage(WebNet* net);
        
    private:
		void prepareBuildersForPDMatrices(WebNet* net, CRSBuilder& builderP, CRSBuilder& builderD);
        void updateNetWithRanks(vector<double> eigenvector, WebNet* net);
        
        double _teletransporting;
        double _cutTolerance;
};

class HITS : public RankingAlgorithm {
    public:
		HITS(double cutTolerance) : _cutTolerance(cutTolerance) {}
        void RankPage(WebNet* net);
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
		
		double _cutTolerance;
		AuthorityHubWeightVectors Iterate(CRSMatrix& adjacencyMatrix, CRSMatrix& transposedAdjacencyMatrix);
		
};

class InDegree : public RankingAlgorithm {
    public:
        void RankPage(WebNet* net);
        ~InDegree(){};
};

// STATISTICS

class RunStatistics {
	public:
		virtual ~RunStatistics(){}
		virtual void save() = 0;
		void add(int iteration, double manhattanDifference, double norm2Difference);
	
	protected:
		vector<int> _iterations;
		vector<double> _manhattanDifference;
		vector<double> _norm2Difference;
};

class PageRankRunStatistic : public RunStatistics{
    public:
		PageRankRunStatistic(int amountOfNodes, double tolerance, double teletransporting) : _amountOfNodes(amountOfNodes), _tolerance(tolerance), _teletransporting(teletransporting) {}
		~PageRankRunStatistic();
		void save();
    
    private:
		int _amountOfNodes;
		double _tolerance;
		double _teletransporting;
};

class HITSRunStatistic : public RunStatistics{
    public:
		HITSRunStatistic(int amountOfNodes) : _amountOfNodes(amountOfNodes) {}
		~HITSRunStatistic();
		void save();
    
    private:
		int _amountOfNodes;
};

#endif
