#include "ranking_algorithm.h"

using namespace std;

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
    srand(42);
    for(int i=0;i<n;i++)
        randomVector.push_back((double)rand()); //TODO: this should be random!!!!
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

PageRank :: ~PageRank(){}

vector<double> substractVectors(vector<double>& v1, vector<double>& v2){
	vector<double> substraction = vector<double>(v1.size(), 0);
	for (int i =0; i < (int)v1.size(); i++)
		substraction[i] = v1[i] - v2[i];
		
	return substraction;
}

double differenceBetweenSolutions(vector<double>& x, vector<double>& old_x){
	vector<double> subSol = substractVectors(x,old_x);
	return calculateNorm2(subSol);
}

vector<double> sumVectors(vector<double>& v1, vector<double>& v2){
	vector<double> substraction = vector<double>(v1.size(), 0);
	for (int i =0; i < (int)v1.size(); i++)
		substraction[i] = v1[i] + v2[i];
		
	return substraction;
}

void PageRank :: RankPage(WebNet* net, int amountOfIterations){
    
    //CRSMatrix adjacencyMatrix = createAdjacencyMatrix(net); 
    
	CRSBuilder builderP, builderD;
    list<WebPage*>::iterator itNetPages = (net->webPages())->begin();
    
	for (itNetPages; itNetPages != (net->webPages())->end(); ++itNetPages){
        list<int>* linkedPageIds = ((*itNetPages)->listOfLinkedWebPagesIds());
        list<int>::iterator itIdLinkedPages = linkedPageIds->begin();
        
        // Construction of D matrix (D = vd^t, v_i= 1/n. if (n_i == 0) d_i = 1/n else d_i = 0. So, row_i(D) = 1/n * d)
        double pageCount = (double)linkedPageIds->size();
        for (int i = 0; i < net->amountOfNodes(); i++){
			double valueToPut = 0;
			if (pageCount == 0) valueToPut = 1/(double)net->amountOfNodes();
			builderD.AddElementAt(i,(*itNetPages)->pageId() - 1, valueToPut);
		}
		
        for (itIdLinkedPages; itIdLinkedPages != linkedPageIds->end(); ++itIdLinkedPages){
			int referencedPage = (*itNetPages)->pageId() - 1;
            int page = *itIdLinkedPages - 1;
            builderP.AddElementAt(page, referencedPage, 1.0/pageCount);
		}
	}
    
    CRSMatrix P = builderP.Build(net->amountOfNodes(), net->amountOfNodes());
    CRSMatrix D = builderD.Build(net->amountOfNodes(), net->amountOfNodes());
    
    int pageCount = net->amountOfNodes();
    vector<double> x = createRandomVectorOfSize(pageCount);
    
    // We must find a x_0 initial vector provided that x_i >= 0 && sum(xi) = 1. Therefore, we apply norm L1 to x (scaling it by its inverse sum)
    scaleBy(1/sumElements(x), x);
    
    // We keep an old solution to keep track of the cutTolerance
    vector<double> old_x = x;
    do
    {
        /*  We need to solve Ax=x
            Ax=x with model enrichment <==> 
            [cA + (1-c)E]x = x <==>
            c(Ax) + (1-c)Ex = x <==>
            c(Ax) + (1-c)(v_1/n)(v_1^t)x = x (v_1/n is a vector of 1/n, v_1 is a vector of ones) <==> 
            c(Ax) + (v_(1-c)/n)(v_1^t)x = x  <==> 
            c(Ax) + [((1-c)/n).sum(x_i) foreach row] = x
        */
       
        old_x = x;
        double proportion = (1.0 - _teletransporting)/(double)pageCount;
        double proportionalEigenvectorSum = sumElements(x) * proportion;
        
        vector<double> Px = P.Multiply(x);      // performs Px
        vector<double> Dx = D.Multiply(x);      // performs Dx
        x = sumVectors(Px,Dx);
        scaleBy(_teletransporting, x);                           // performs c(Ax)
        addConstantToEachElement(proportionalEigenvectorSum, x); // adds [((1-c)/n).sum(x_i) foreach row]
        //normalizeVector(x);
    } while(differenceBetweenSolutions(x, old_x) >= _cutTolerance);
    
    
    updateNetWithRanks(x, net);
}

void PageRank :: updateNetWithRanks(vector<double> eigenvector, WebNet* net)
{
    list<WebPage*>* webPages = net->webPages();
    list<WebPage*>::iterator itNetPages = webPages->begin();
	for (itNetPages; itNetPages != webPages->end(); ++itNetPages){
		int pageId = (*itNetPages)->pageId() - 1;
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
			int referencedPage = (*itNetPages)->pageId() - 1;
            int page = *itIdLinkedPages - 1;
            builder.AddElementAt(page, referencedPage, 1.0/pageCount);
		}
	}
    
    return  builder.Build(net->amountOfNodes(), net->amountOfNodes());
}


//bool orderMostImportantFirst (int i,int j) { return (i>j); }
HITS::AuthorityHubWeightVectors HITS::Iterate(CRSMatrix& adjacencyMatrix, CRSMatrix& transposedAdjacencyMatrix, int amountOfIterations){
	vector<double> authorityWeightVector = vector<double>(adjacencyMatrix.amountOfColumns(), 1.0);
	vector<double> hubWeightVector = vector<double>(adjacencyMatrix.amountOfColumns(), 1.0);
	HITS::AuthorityHubWeightVectors authorityHubWeightVectors(authorityWeightVector, hubWeightVector);

	for (int i = 1; i <= amountOfIterations; i++){
		authorityHubWeightVectors.authorityWeightVector = transposedAdjacencyMatrix.Multiply(authorityHubWeightVectors.hubWeightVector);
		authorityHubWeightVectors.hubWeightVector = adjacencyMatrix.Multiply(authorityHubWeightVectors.authorityWeightVector);
		authorityHubWeightVectors.normalizeVectors();
			
	}
	
	return authorityHubWeightVectors;
}

void HITS :: RankPage(WebNet* net, int amountOfIterations){
	
	/** Create adjacency matrix **/
	CRSBuilder builder, builderTransposed;
	for (list<WebPage*>::iterator itNetPages = (net->webPages())->begin(); itNetPages != (net->webPages())->end(); ++itNetPages){
		for (list<int>::iterator itIdLinkedPages = ((*itNetPages)->listOfLinkedWebPagesIds())->begin(); itIdLinkedPages != ((*itNetPages)->listOfLinkedWebPagesIds())->end(); ++itIdLinkedPages){
			int fromPageId = (*itNetPages)->pageId();
			int toPageId = *itIdLinkedPages;
			fromPageId--; toPageId--;
			builder.AddElementAt(fromPageId, toPageId, 1.0);
			builderTransposed.AddElementAt(toPageId, fromPageId, 1.0);
		}
	}
    
    int adjMatrixSize = net->amountOfNodes();
    CRSMatrix adjacencyMatrix = builder.Build(adjMatrixSize, adjMatrixSize);
    CRSMatrix transposedAdjacencyMatrix = builderTransposed.Build(adjMatrixSize, adjMatrixSize);

    /** Find weights (hub & authority) **/
	AuthorityHubWeightVectors authorityHubWeightVectors = Iterate(adjacencyMatrix, transposedAdjacencyMatrix, amountOfIterations);

	
	/** Rank WebPages **/ 
	for (list<WebPage*>::iterator itNetPages = net->webPages()->begin(); itNetPages != net->webPages()->end(); ++itNetPages){
		HITSRank* webPageRanking = new HITSRank(authorityHubWeightVectors.authorityWeightVector[(*itNetPages)->pageId()-1], authorityHubWeightVectors.hubWeightVector[(*itNetPages)->pageId()-1]);
		(*itNetPages)->rankWebPage( (Rank*)webPageRanking );
	}
	
	/// Show results
	/*authorityHubWeightVectors.sortVectorsMostImportantFirst();
	for (int i = 0; i < amountOfResultsToShow; i++){
		
	}*/
}

void InDegree :: RankPage(WebNet* net, int amountOfIterations){
	
	/** Calculate In-Degree for each page in a separate array **/
	vector<int> inDegreeForAllPages = vector<int>(net->amountOfNodes(), 0);
	
	for (list<WebPage*>::iterator itNetPages = (net->webPages())->begin(); itNetPages != (net->webPages())->end(); ++itNetPages){
		for (list<int>::iterator itIdLinkedPages = ((*itNetPages)->listOfLinkedWebPagesIds())->begin(); itIdLinkedPages != ((*itNetPages)->listOfLinkedWebPagesIds())->end(); ++itIdLinkedPages){
			inDegreeForAllPages[*itIdLinkedPages - 1]++;
		}
	}
	
	/** Rank WebPages **/ 	
	for (list<WebPage*>::iterator itNetPages = (net->webPages())->begin(); itNetPages != (net->webPages())->end(); ++itNetPages){
		PageRankInDegreeRank* webPageRanking = new PageRankInDegreeRank(inDegreeForAllPages[(*itNetPages)->pageId()-1]);
		(*itNetPages)->rankWebPage( (Rank*)webPageRanking );
	}
}
