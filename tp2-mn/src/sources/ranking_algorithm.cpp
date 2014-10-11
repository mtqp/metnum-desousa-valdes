#include "ranking_algorithm.h"

using namespace std;

/******* VECTOR UTILITES *******/ 

void scaleBy(double aScalingFactor, vector<double>& aVector){
    for (int i = 0; i < (int)aVector.size(); i++)
	{
		aVector[i] = aVector[i]*aScalingFactor;
	}
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
	double sum = 0;
    
	for (int i = 0; i < (int)aVector.size(); i++)
	{
		sum += aVector[i];
	}
    
    return sum;
}

double manhattanNorm(vector<double>& aVector)
{
	double sum = 0;
    
	for (int i = 0; i < (int)aVector.size(); i++)
	{
		sum += abs(aVector[i]);
	}
    
    return sum;
}

vector<double> substractVectors(vector<double>& v1, vector<double>& v2){
	vector<double> substraction = vector<double>(v1.size(), 0);
	for (int i =0; i < (int)v1.size(); i++)
		substraction[i] = v1[i] - v2[i];
		
	return substraction;
}

vector<double> sumVectors(vector<double>& v1, vector<double>& v2){
	vector<double> substraction = vector<double>(v1.size(), 0);
	for (int i =0; i < (int)v1.size(); i++)
		substraction[i] = v1[i] + v2[i];
		
	return substraction;
}

void addConstantToEachElement(double aConstant, vector<double>& aVector){
    for (int i = 0; i < (int)aVector.size(); i++)
	{
		aVector[i] += aConstant;
	}
}

vector<double> createRandomVectorOfSize(int n, unsigned int srand_seed){
    vector<double> randomVector;
    srand(srand_seed);
    for(int i=0;i<n;i++)
        randomVector.push_back((double)rand());
    return randomVector;
}

double norm2DifferenceBetweenSolutions(vector<double>& x, vector<double>& old_x){
	vector<double> subSol = substractVectors(x,old_x);
	return calculateNorm2(subSol);
}

//////////STATISTICS/////////////
void RunStatistics :: add(int iteration, double manhattanDifference, double norm2Difference)
{
    _iterations.push_back(iteration);
    _manhattanDifference.push_back(manhattanDifference);
    _norm2Difference.push_back(norm2Difference);
}

PageRankRunStatistic :: ~PageRankRunStatistic(){}

void PageRankRunStatistic :: save()
{
    std::ostringstream fileName;
    fileName << "tests/page_rank_statistics/tol" << _tolerance << "_teletrans" << _teletransporting << "_nAmount" << _amountOfNodes << ".stat";
    const std::string tmp = fileName.str();
    const char* cFileName = tmp.c_str();
    cout << "Saving statistics file: " << fileName.str() << endl;

    ofstream file;
	file.open(cFileName);

	if (file.is_open()){
        file << _amountOfNodes << endl;
        file << _tolerance << endl;
        file << _teletransporting << endl;
        
        for(int i=0;i< (int)_iterations.size();i++) {
            file << _iterations[i] << " " << _manhattanDifference[i] << " " << _norm2Difference[i] << endl;
        }
    }
    
    file.close();
    
    cout << "Statistics saved" << endl;
}

HITSRunStatistic :: ~HITSRunStatistic(){}

void HITSRunStatistic :: save()
{
    std::ostringstream fileName;
    fileName << "tests/HITS_statistics/nAmount" << _amountOfNodes << ".stat";
    const std::string tmp = fileName.str();
    const char* cFileName = tmp.c_str();
    cout << "Saving statistics file: " << fileName.str() << endl;

    ofstream file;
	file.open(cFileName);

	if (file.is_open()){
        file << _amountOfNodes << endl;
        
        // AUTH
        for(int i=0;i< (int)_iterations.size(); i+=2) {
            file << _iterations[i] << "\t" << _manhattanDifference[i] << "\t" << _norm2Difference[i] << endl;
        }
        file << endl;
        // HUBS
		for(int i=1;i< (int)_iterations.size(); i+=2) {
            file << _iterations[i] << "\t" << _manhattanDifference[i] << "\t" << _norm2Difference[i] << endl;
        }
    }
    
    file.close();
    
    cout << "Statistics saved" << endl;
}

/******* PAGERANK *******/ 

PageRank :: ~PageRank(){}

void PageRank::prepareBuildersForPDMatrices(WebNet* net, CRSBuilder& builderP, CRSBuilder& builderD){
	list<WebPage*>::iterator itNetPages = (net->webPages())->begin();
    
	for (itNetPages; itNetPages != (net->webPages())->end(); ++itNetPages){
        list<int>* linkedPageIds = ((*itNetPages)->listOfLinkedWebPagesIds());
        list_iterator itIdLinkedPages = linkedPageIds->begin();
        
        // Construction of D matrix (D = vd^t, v_i= 1/n. if (n_i == 0) d_i = 1/n; else d_i = 0. So, row_i(D) = 1/n * d)
        double pageCount = (double)linkedPageIds->size();
        for (int i = 0; i < net->amountOfNodes(); i++){
			double valueToPut = 0;
			if (pageCount == 0) valueToPut = 1/(double)net->amountOfNodes();
			builderD.AddElementAt(i,(*itNetPages)->pageId() - 1, valueToPut);
		}
		
		// Construction of P matrix ( P: if (wij = 1) pij = 1/n_j; else 0 )
        for (itIdLinkedPages; itIdLinkedPages != linkedPageIds->end(); ++itIdLinkedPages){
			int referencedPage = (*itNetPages)->pageId() - 1;
            int page = *itIdLinkedPages - 1;
            builderP.AddElementAt(page, referencedPage, 1.0/pageCount);
		}
	}
}

void PageRank :: RankPage(WebNet* net){
	
	// Preparing builders for P and D matrices	
	CRSBuilder builderP, builderD;
	prepareBuildersForPDMatrices(net, builderP, builderD);
    
    // Build matrices
    CRSMatrix P = builderP.Build(net->amountOfNodes(), net->amountOfNodes());
    CRSMatrix D = builderD.Build(net->amountOfNodes(), net->amountOfNodes());
    
    // Creation of initial solution x (rand vector) with constant seed
    unsigned int srand_seed = 42;
    int pageCount = net->amountOfNodes();
    vector<double> x = createRandomVectorOfSize(pageCount, srand_seed);
    
    /* We must find a x_0 initial vector that complies both x_i >= 0 && sum(xi) = 1. 
     * Therefore, we apply norm L1 to x (scaling it by its inverse sum)  */
    scaleBy(1/sumElements(x), x);
    
    // We keep an old solution to keep track of the cutTolerance
    vector<double> old_x = x;
    
    PageRankRunStatistic statistics(net->amountOfNodes(), _cutTolerance, _teletransporting);
    
    int iteration = 0;
    
    do {
        /*  We need to solve Ax=x
            Ax=x with model enrichment <==> 
            [c(P+D) + (1-c)E]x = x <==>
            c((P+D)x) + (1-c)Ex = x <==>
            c(Px+Dx) + (1-c)(v_1/n)(v_1^t)x = x (v_1/n is a vector of 1/n, v_1 is a vector of ones) <==> 
            c(Px+Dx) + (v_(1-c)/n)(v_1^t)x = x  <==> 
            c(Px+Dx) + [((1-c)/n).sum(x_i) foreach row] = x
        */
       
        old_x = x;
        double proportion = (1.0 - _teletransporting)/(double)pageCount;
        double proportionalEigenvectorSum = sumElements(x) * proportion;
        
        vector<double> Px = P.Multiply(x);      // performs Px
        vector<double> Dx = D.Multiply(x);      // performs Dx
        x = sumVectors(Px,Dx);
        scaleBy(_teletransporting, x);                           // performs c(Px+Dx)
        addConstantToEachElement(proportionalEigenvectorSum, x); // adds [((1-c)/n).sum(x_i) foreach row]
        
        vector<double> vectorDifference = substractVectors(x, old_x);
        double norm2Difference = norm2DifferenceBetweenSolutions(x, old_x);
        double manhattanDifference = manhattanNorm(vectorDifference);
        statistics.add(iteration, manhattanDifference, norm2Difference);

        iteration++;
    } while(norm2DifferenceBetweenSolutions(x, old_x) >= _cutTolerance && iteration < 5000); //It could not end due to numerical error
    
    if(iteration >= 5000) {
        cout << "Run out of iterations - Teletransporting value: " 
             << _teletransporting << " - Cut tolerance: " 
             << _cutTolerance << endl;
    }
    
    statistics.save();
   
    // Set rankings in webPages
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

/******* HITS *******/ 


HITS::AuthorityHubWeightVectors HITS::Iterate(CRSMatrix& adjacencyMatrix, CRSMatrix& transposedAdjacencyMatrix){
	vector<double> authorityWeightVector = vector<double>(adjacencyMatrix.amountOfColumns(), 1.0);
	vector<double> hubWeightVector = vector<double>(adjacencyMatrix.amountOfColumns(), 1.0);
	HITS::AuthorityHubWeightVectors authorityHubWeightVectors(authorityWeightVector, hubWeightVector);

	// Statistics
	vector<double> oldAuthVector, oldHubVector;
	int iteration = 0;
	
    HITSRunStatistic statistics(adjacencyMatrix.amountOfColumns());
	
	do {
		oldAuthVector = authorityHubWeightVectors.authorityWeightVector;
		oldHubVector = authorityHubWeightVectors.hubWeightVector;
		authorityHubWeightVectors.authorityWeightVector = transposedAdjacencyMatrix.Multiply(authorityHubWeightVectors.hubWeightVector);
		authorityHubWeightVectors.hubWeightVector = adjacencyMatrix.Multiply(authorityHubWeightVectors.authorityWeightVector);
		authorityHubWeightVectors.normalizeVectors();
		
		// Statistics AUTH
		vector<double> vectorDifference = substractVectors(authorityHubWeightVectors.authorityWeightVector, oldAuthVector);
        double norm2Difference = norm2DifferenceBetweenSolutions(authorityHubWeightVectors.authorityWeightVector, oldAuthVector);
        double manhattanDifference = manhattanNorm(vectorDifference);
        statistics.add(iteration, manhattanDifference, norm2Difference);
        
        // Statistics HUB
		vectorDifference = substractVectors(authorityHubWeightVectors.hubWeightVector, oldHubVector);
        norm2Difference = norm2DifferenceBetweenSolutions(authorityHubWeightVectors.hubWeightVector, oldHubVector);
        manhattanDifference = manhattanNorm(vectorDifference);
        statistics.add(iteration, manhattanDifference, norm2Difference);
        iteration++;
        
	} while (norm2DifferenceBetweenSolutions(authorityHubWeightVectors.hubWeightVector, oldHubVector) >= _cutTolerance || norm2DifferenceBetweenSolutions(authorityHubWeightVectors.authorityWeightVector, oldAuthVector) >= _cutTolerance);
	
	statistics.save();
	
	return authorityHubWeightVectors;
}

void HITS :: RankPage(WebNet* net){
	
	// Create adjacency matrix
	CRSBuilder builder, builderTransposed;
	list<WebPage*>* listOfWebPages = net->webPages();
	for (list<WebPage*>::iterator itNetPages = listOfWebPages->begin(); itNetPages != listOfWebPages->end(); ++itNetPages){
		list<int>* listOfLinksId = (*itNetPages)->listOfLinkedWebPagesIds();
		for (list_iterator itIdLinkedPages = listOfLinksId->begin(); itIdLinkedPages != listOfLinksId->end(); ++itIdLinkedPages){
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

    // Find weights (hub & authority) <==> rank webPages
	AuthorityHubWeightVectors authHubWeightVectors = Iterate(adjacencyMatrix, transposedAdjacencyMatrix);

	
	// Set rankings in webPages
	for (list<WebPage*>::iterator itNetPages = net->webPages()->begin(); itNetPages != net->webPages()->end(); ++itNetPages){
		int pageId = (*itNetPages)->pageId()-1;
		HITSRank* webPageRanking = new HITSRank(authHubWeightVectors.authorityWeightVector[pageId], authHubWeightVectors.hubWeightVector[pageId]);
		(*itNetPages)->rankWebPage( (Rank*)webPageRanking );
	}
}

void InDegree :: RankPage(WebNet* net){
	
	// Calculate In-Degree for each page in a separate array (bucket)
	vector<int> inDegreeForAllPages = vector<int>(net->amountOfNodes(), 0);
	
	list<WebPage*>* listOfWebPages = net->webPages();
	for (list<WebPage*>::iterator itNetPages = listOfWebPages->begin(); itNetPages != listOfWebPages->end(); ++itNetPages){
		list<int>* listOfLinksId = (*itNetPages)->listOfLinkedWebPagesIds();
		for (list_iterator itIdLinkedPages = listOfLinksId->begin(); itIdLinkedPages != listOfLinksId->end(); ++itIdLinkedPages){
			inDegreeForAllPages[*itIdLinkedPages - 1]++;
		}
	}
	
	// Set rankings in webPages
	for (list<WebPage*>::iterator itNetPages = (net->webPages())->begin(); itNetPages != (net->webPages())->end(); ++itNetPages){
		int pageId = (*itNetPages)->pageId()-1;
		PageRankInDegreeRank* webPageRanking = new PageRankInDegreeRank(inDegreeForAllPages[pageId]);
		(*itNetPages)->rankWebPage( (Rank*)webPageRanking );
	}
}
