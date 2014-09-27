#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>

//TODO DELETE WHAT WE ARE NOT USING

//TODO: REFACTOR INTO MULTIPLE FILES!

using namespace std;

enum InstanceType { STANFORD, TORONTO }; // Stanford = 0, Toronto = 1
enum AlgorithmType { PAGERANK, HITS, INDEG }; // PageRank = 0, HITS = 1, INDEG = 2

class RealMatrix { //TODO: A good implementation would use templates

    public:
        RealMatrix(int n, int m);   
        virtual vector<double>& Multiply(vector<double> aVector){} //TODO: We should overload * operator - CANT REMEMBER WHERE THE CONST GOES... DON'T HAVE TIME FOR THAT SHIT
        virtual double ElementAt(int i, int j){} //TODO: We should overload [] operator

    protected:
        int _n;
        int _m;
};

RealMatrix :: RealMatrix(int n, int m){
    _n = n;
    _m = m;
}

class CSCMatrix {//: public RealMatrix{ //Compressed sparse column
    public:
        CSCMatrix(int n, int m);//TODO complete with data structure creation
        
        vector<double>& Multiply(vector<double> aVector){ 
			//c*A +  (1-c/N)
   			//multiply knowing that if element i.j is zero, we should user (1-c)/n
            vector<double> dummy;
            return dummy;
        }
        double ElementAt(int i, int j) { return 0.0;}

		void AddElementAt(int i, int j, double value) {}

};

CSCMatrix :: CSCMatrix(int n, int m)/*: RealMatrix(n, m)*/{}

class WebPage {
    public:
        WebPage(string pageName) { _name = pageName;} //add other relevant information
        
        string PageName(){ return _name;}
        
    private:
        string _name;
        int _id;
        List<int> _listOfLinkedWebPagesIds;
        Rank _ranking;
};

class RankingAlgorithm { //should be created with a PerformanceAnalyzer object
    public:
        virtual void Rank(vector<WebPage> pages){} //i think this won't be void but a orderd list of ranked pages.
        
};


class PageRank : public RankingAlgorithm {
    public:
        void Rank(vector<WebPage> pages);
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
        void Rank(vector<WebPage> pages);
};

class InDegree : public RankingAlgorithm {
    public:
        void Rank(vector<WebPage> pages);
};


class ParsingAlgorithm {
    public:
        virtual List<WebPage> ParseFile(string pathToFile) = 0;
        virtual void SaveRankTo(string savingFile, Rank aRank) = 0;
        
};

class TorontoParsing : ParsingAlgorithm{
	public:
		List<WebPage> ParseFile(string pathToFile);
        void SaveRankTo(string savingFile, Rank aRank);	
};

List<WebPage> :: TorontoParsing ParseFile(string pathToFile){
	List<WebPage> dummy;
	return dummy;
}

class StanfordParsing : ParsingAlgorithm{
	public:
		List<WebPage> ParseFile(string pathToFile);
        void SaveRankTo(string savingFile, Rank aRank);	
};

List<WebPage> :: StanfordParsing ParseFile(string pathToFile){
	List<WebPage> dummy;
	return dummy;
}

ParsingAlgorithm CreateParsingAlgorithmFromParameter(InstanceType instanceType){
	switch (instanceType){
		case STANFORD:
			ParsingAlgorithm stanfordParsingAlgorithm = StanfordParsing();
			return stanfordParsingAlgorithm;
		break;
		
		case TORONTO:
			ParsingAlgorithm torontoParsingAlgorithm = TorontoParsing();
			return torontoParsingAlgorithm;
		break;
	}
}

RankingAlgorithm CreateRankingAlgorithmFromParameter(AlgorithmType algorithmType){
	switch (algorithmType){
		case PAGERANK:
			RankingAlgorithm pageRankAlgorithm = PageRank();
			return pageRankAlgorithm;
		break;
		
		case HITS:
			RankingAlgorithm HITSAlgorithm = HITS();
			return HITSAlgorithm;
		break;
		
		case INDEG:
			RankingAlgorithm InDegAlgorithm = InDegree();
			return InDegAlgorithm;
		break;
	}
}


int main(int argc, char *argv[]) {	
/*  --Pseudocodigo de pasos
	TeletransportingValue c = GetTeletransportingValue(second argument)
	double tolerance = RetrieveToleranceFrom(fifth argument)

	RankingAlgorithm rankingAlgorithm = CreateRankingAlgorithmFromParameter(first argument) //PageRangk || HITS || INDEG
	ParsingAlgorithm parsingAlgorithm = CreateParsingAlgorithmFromParameter(third argument)

	List<WebPage> pages = parsingAlgorithm.ParseFile(pathRetrievedFromFourthArgument)
	
	Rank aRank = rankingAlgorithm.rank(pages);
	
	parsingAlgorithm.SaveRankTo(savingFile, aRank);
	
	

/*	char* input_file = argv[1];
	char* output_file = argv[2];
	int bandImplementation = atoi(argv[3]);
*/
/*    CSCMatrix* matrix = new CSCMatrix(5,5);
    cout << "A dummy implementation that returns: " << matrix->ElementAt(0,0) << endl;
*/
	return 0;
}
