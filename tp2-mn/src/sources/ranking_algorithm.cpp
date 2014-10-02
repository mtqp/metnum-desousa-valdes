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

/*TODO: 
    this should be in a class linked to vectors... 
    maybe we shouldnt have used vectors but our own implementation
*/

void scaleBy(double aScalingFactor, vector<double>& aVector){
    for (int i = 0; i < (int)aVector.size(); i++)
	{
		aVector[i] = aVector[i]*aScalingFactor;
	}
}

vector<double> createRandomVectorOfSize(int n){
    vector<double> randomVector;
    for(int i=0;i<n;i++)
        randomVector.push_back(0.0); //TODO: this should be random!!!!
    return randomVector;
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
	
    scaleBy(1.0/aVectorNorm2, aVector);
}

double sumElements(vector<double>& aVector){
//TODO: CHECK - SHOULD WE NORMALIZE?
	double sum = 0;
    
	for (int i = 0; i < (int)aVector.size(); i++)
	{
		sum += aVector[i];
	}
    
    return sum;
}

void addConstantToEachElement(double aConstant, vector<double>& aVector){
    for (int i = 0; i < (int)aVector.size(); i++)
	{
		aVector[i] += aConstant;
	}
}

class RankingAlgorithm { //should be created with a PerformanceAnalyzer object
    public:
        virtual void RankPage(WebNet* net, int amountOfIterations) = 0; //i think this won't be void but a orderd list of ranked pages.
        virtual ~RankingAlgorithm(){};
        
};

class PageRank : public RankingAlgorithm {
    public:
        PageRank(double teletransportingProbability) : _teletransporting(teletransportingProbability) {}
        ~PageRank(){};
    
        void RankPage(WebNet* net, int amountOfIterations);
        
    private:
        CRSMatrix createAdjacencyMatrix(WebNet* net);
        void updateNetWithRanks(vector<double> eigenvector, WebNet* net);
        
        double _teletransporting;
};

void PageRank :: RankPage(WebNet* net, int amountOfIterations){
    
    CRSMatrix adjacencyMatrix = createAdjacencyMatrix(net); 
    
    int pageCount = net->amountOfNodes();
    vector<double> lambdaOneEigenvector = createRandomVectorOfSize(pageCount);
    
    for(int iteration=0; iteration<amountOfIterations; iteration++)
    {
        /*  We need to solve Ax=x
            Ax=x with model enrichment <==> 
            [cA + (1-c)E]x = x <==>
            c(Ax) + (1-c)Ex = x <==>
            c(Ax) + (1-c)(v_1/n)(v_1^t)x = x (v_1/n is a vector of 1/n, v_1 is a vector of ones) <==> 
            c(Ax) + (v_(1-c)/n)(v_1^t)x = x  <==> 
            c(Ax) + [((1-c)/n).sum(x_i) foreach row] = x
            
            Remembering to normalize x on each iteration.
        */
        
        normalizeVector(lambdaOneEigenvector);
        
        double proportion = (1.0 - _teletransporting)/(double)pageCount;
        double proportionalEigenvectorSum = sumElements(lambdaOneEigenvector) * proportion;
        
        lambdaOneEigenvector = adjacencyMatrix.Multiply(lambdaOneEigenvector);      // performs Ax
        scaleBy(_teletransporting, lambdaOneEigenvector);                           // performs c(Ax)
        addConstantToEachElement(proportionalEigenvectorSum, lambdaOneEigenvector); // adds [((1-c)/n).sum(x_i) foreach row]
        
    }
    
    updateNetWithRanks(lambdaOneEigenvector, net);
}

void PageRank :: updateNetWithRanks(vector<double> eigenvector, WebNet* net)
{
    list<WebPage*>* webPages = net->webPages();
    list<WebPage*>::iterator itNetPages = webPages->begin();
	for (itNetPages; itNetPages != webPages->end(); ++itNetPages){
		int pageId = (*itNetPages)->pageId();
        double rank = eigenvector[pageId];
        
        PageRankInDegreeRank* ranking = new PageRankInDegreeRank(rank);
		(*itNetPages)->rankWebPage( (Rank*)ranking );
	}
}

CRSMatrix PageRank :: createAdjacencyMatrix(WebNet* net)
{
    CRSBuilder builder;
    list<WebPage*>::iterator itNetPages = (net->webPages())->begin();
    
	for (itNetPages; itNetPages != (net->webPages())->end(); ++itNetPages){
        list<int>* linkedPageIds = ((*itNetPages)->listOfLinkedWebPagesIds());
        list<int>::iterator itIdLinkedPages = linkedPageIds->begin();
        
        double pageCount = (double)linkedPageIds->size();
		
        for (itIdLinkedPages; itIdLinkedPages != linkedPageIds->end(); ++itIdLinkedPages){
			int referencedPage = (*itNetPages)->pageId();
            int page = *itIdLinkedPages;
            builder.AddElementAt(page, referencedPage, 1.0/pageCount);
		}
	}
    
    return  builder.Build(net->amountOfNodes(), net->amountOfNodes());
}


//bool orderMostImportantFirst (int i,int j) { return (i>j); }

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
		
		AuthorityHubWeightVectors Iterate(CRSMatrix& adjacencyMatrix, CRSMatrix& transposedAdjacencyMatrix, int amountOfIterations){
			vector<double> authorityWeightVector = vector<double>(adjacencyMatrix.amountOfColumns(), 1);
			vector<double> hubWeightVector = vector<double>(adjacencyMatrix.amountOfColumns(), 1);
			AuthorityHubWeightVectors authorityHubWeightVectors(authorityWeightVector, hubWeightVector);
			
			for (int i = 1; i <= amountOfIterations; i++){
				authorityHubWeightVectors.authorityWeightVector = transposedAdjacencyMatrix.Multiply(authorityHubWeightVectors.hubWeightVector); // Falta hacer la traspuesta de adjacencyMatrix!!
				authorityHubWeightVectors.hubWeightVector = adjacencyMatrix.Multiply(authorityHubWeightVectors.authorityWeightVector);
				authorityHubWeightVectors.normalizeVectors();
			}
			
			return authorityHubWeightVectors;
		}
};

void HITS :: RankPage(WebNet* net, int amountOfIterations){
	
	/** Create adjacency matrix **/
	CRSBuilder builder, builderTransposed;
	for (list<WebPage*>::iterator itNetPages = (net->webPages())->begin(); itNetPages != (net->webPages())->end(); ++itNetPages){
		for (list<int>::iterator itIdLinkedPages = ((*itNetPages)->listOfLinkedWebPagesIds())->begin(); itIdLinkedPages != ((*itNetPages)->listOfLinkedWebPagesIds())->end(); ++itIdLinkedPages){
			builder.AddElementAt((*itNetPages)->pageId(), *itIdLinkedPages, 1.0);
			builderTransposed.AddElementAt(*itIdLinkedPages, (*itNetPages)->pageId(), 1.0);
		}
	}
    
    int adjMatrixSize = net->amountOfNodes();
    CRSMatrix adjacencyMatrix = builder.Build(adjMatrixSize, adjMatrixSize);
    CRSMatrix transposedAdjacencyMatrix = builderTransposed.Build(adjMatrixSize, adjMatrixSize);
    
    /** Find weights (hub & authority) **/
	AuthorityHubWeightVectors authorityHubWeightVectors = Iterate(adjacencyMatrix, transposedAdjacencyMatrix, amountOfIterations);
	
	
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
        void RankPage(WebNet* net, int amountOfIterations);
        ~InDegree(){};
};

void InDegree :: RankPage(WebNet* net, int amountOfIterations){
	
	/** Calculate In-Degree for each page in a separate array **/
	vector<int> inDegreeForAllPages = vector<int>(net->amountOfNodes(), 0);
	
	for (list<WebPage*>::iterator itNetPages = (net->webPages())->begin(); itNetPages != (net->webPages())->end(); ++itNetPages){
		for (list<int>::iterator itIdLinkedPages = ((*itNetPages)->listOfLinkedWebPagesIds())->begin(); itIdLinkedPages != ((*itNetPages)->listOfLinkedWebPagesIds())->end(); ++itIdLinkedPages){
			inDegreeForAllPages[*itIdLinkedPages]++;
		}
	}
	
	/** Rank WebPages **/ 	
	for (list<WebPage*>::iterator itNetPages = (net->webPages())->begin(); itNetPages != (net->webPages())->end(); ++itNetPages){
		PageRankInDegreeRank* webPageRanking = new PageRankInDegreeRank(inDegreeForAllPages[(*itNetPages)->pageId()]);
		(*itNetPages)->rankWebPage( (Rank*)webPageRanking );
	}
}

#endif
