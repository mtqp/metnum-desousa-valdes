#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>

//TODO DELETE WHAT WE ARE NOT USING

//TODO: REFACTOR INTO MULTIPLE FILES!

using namespace std;

enum InstanceType { STANFORD, TORONTO }; // Stanford = 0, Toronto = 1
enum AlgorithmType { PAGERANK, HITSALG, INDEG }; // PageRank = 0, HITSALG = 1, INDEG = 2

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

class Rank {
	
};

class WebPage {
    public:
        WebPage(int nodeId, list<int>* listOfLinkedWebPagesIds) : _id(nodeId), _listOfLinkedWebPagesIds(listOfLinkedWebPagesIds) {}
        
        const char* PageName(){ return _name;}
        int getId(){ return _id; }
        
    private:
        const char* _name;
        int _id;
        list<int>* _listOfLinkedWebPagesIds;
        Rank _ranking;
};

class WebNet{
	public:
		WebNet(int amountOfNodes, int amountOfEdges, list<WebPage*>* webPages) : _amountOfNodes(amountOfNodes), _amountOfEdges(amountOfEdges), _webPages(webPages) {}; 
	
	private: 
		int _amountOfNodes;
		int _amountOfEdges;
		list<WebPage*>* _webPages; 
};

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


class ParsingAlgorithm {
    public:
        virtual WebNet ParseFile(const char* pathToFile) = 0;//{ cout << "TU VIEJA EN PARSING ALGORITHM"  << endl;};
        virtual void SaveRankTo(const char* savingFile, Rank aRank, AlgorithmType algorithmType){};
        virtual ~ParsingAlgorithm(){};
        
};

class TorontoParsing : public ParsingAlgorithm{
	public:
		WebNet ParseFile(const char* pathToFile);
        void SaveRankTo(const char* savingFile, Rank aRank, AlgorithmType algorithmType){};
		~TorontoParsing(){};
};

WebNet TorontoParsing ::  ParseFile(const char* pathToFile){
	int amountOfNodes, amountOfEdges = 0;
	list<WebPage*>* webPages = new list<WebPage*>();
	ifstream file;
	file.open(pathToFile);
	
	if (file.is_open()){
		int fromNodeId;
		while( !file.eof() )
		{
			file >> fromNodeId ;
			int nodeId = fromNodeId;
			file.ignore(); 				// ignora el :
			int toNodeId;
			file >> toNodeId; 
			
			list<int>* listOfLinkedWebPagesIds = new list<int>();
			while(toNodeId != -1){
				listOfLinkedWebPagesIds->push_back(toNodeId);
				file >> toNodeId;
				if (toNodeId != -1){
					amountOfEdges++;
				}
			}	
			WebPage* node = new WebPage(nodeId, listOfLinkedWebPagesIds);  	//ver
			if(!listOfLinkedWebPagesIds->empty()){
				webPages->push_back(node);
			}
		}
		amountOfNodes = fromNodeId;
		file.close();	
	} else{
		cout << "Unable to open input file" << endl;		
		exit(1);
	}
	
	WebNet net(amountOfNodes, amountOfEdges, webPages);
	return net;
}

class StanfordParsing : public ParsingAlgorithm{
	public:
		WebNet ParseFile(const char* pathToFile);
        void SaveRankTo(const char* savingFile, Rank aRank, AlgorithmType algorithmType){};
        ~StanfordParsing(){};
};

WebNet StanfordParsing :: ParseFile(const char* pathToFile){
	int amountOfNodes, amountOfEdges;
	list<WebPage*>* webPages = new list<WebPage*>();
	ifstream file;
	file.open(pathToFile);
	
	if (file.is_open()){
		list<WebPage*>* webPages = new list<WebPage*>();
		file >> amountOfNodes >> amountOfEdges;
		while( !file.eof() )
		{
			int fromNodeId;
			file >> fromNodeId ;
			int nodeId = fromNodeId;
			
			list<int>* listOfLinkedWebPagesIds = new list<int>();
			do {
				int toNodeId;
				file >> toNodeId;
				listOfLinkedWebPagesIds->push_back(toNodeId);
				fromNodeId = file.peek();
			} while(fromNodeId == nodeId);
			
			WebPage* node = new WebPage(nodeId, listOfLinkedWebPagesIds);
			webPages->push_back(node);
		}
		file.close();	
	}else{
		cout << "Unable to open input file" << endl;		
		exit(1);
	}
	WebNet net(amountOfNodes, amountOfEdges, webPages);
	return net;
}

ParsingAlgorithm* CreateParsingAlgorithmFromParameter(InstanceType instanceType){
	switch (instanceType){
		case STANFORD:{
			StanfordParsing* stanfordParsingAlgorithm = new StanfordParsing();
			return (ParsingAlgorithm*)stanfordParsingAlgorithm;
			break;
		}
		
		case TORONTO:{
			TorontoParsing* torontoParsingAlgorithm = new TorontoParsing();
			return (ParsingAlgorithm*)torontoParsingAlgorithm;
			break;
		}
	}
}

RankingAlgorithm CreateRankingAlgorithmFromParameter(AlgorithmType algorithmType){
	switch (algorithmType){
		case PAGERANK:{
			RankingAlgorithm pageRankAlgorithm = PageRank();
			return pageRankAlgorithm;
			break;
		}
		
		case HITSALG:{
			RankingAlgorithm HITSAlgorithm = HITS();
			return HITSAlgorithm;
			break;
		}
		
		case INDEG:{
			RankingAlgorithm InDegAlgorithm = InDegree();
			return InDegAlgorithm;
			break;
		}
	}
}


int main(int argc, char *argv[]) {	
/*  --Pseudocodigo de pasos
	TeletransportingValue c = GetTeletransportingValue(second argument)
	double tolerance = RetrieveToleranceFrom(fifth argument)

	RankingAlgorithm rankingAlgorithm = CreateRankingAlgorithmFromParameter(first argument) //PageRangk || HITS || INDEG
	ParsingAlgorithm parsingAlgorithm = CreateParsingAlgorithmFromParameter(third argument)

	list<WebPage> pages = parsingAlgorithm.ParseFile(pathRetrievedFromFourthArgument)
	
	Rank aRank = rankingAlgorithm.rank(pages);
	
	parsingAlgorithm.SaveRankTo(savingFile, aRank);*/
	ParsingAlgorithm* parsingAlgorithm = CreateParsingAlgorithmFromParameter(STANFORD);
	WebNet net = parsingAlgorithm->ParseFile("../graph.out");	
	

/*	char* input_file = argv[1];
	char* output_file = argv[2];
	int bandImplementation = atoi(argv[3]);
*/
/*    CSCMatrix* matrix = new CSCMatrix(5,5);
    cout << "A dummy implementation that returns: " << matrix->ElementAt(0,0) << endl;
*/
	return 0;
}
