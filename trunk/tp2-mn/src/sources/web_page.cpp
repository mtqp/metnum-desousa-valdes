#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;

#ifndef __WEB_PAGE__
#define __WEB_PAGE__

class Rank {
	protected:
        virtual void printItSelf(ostream& os) const = 0;
        
	public:
        friend ostream& operator<<(ostream& os, Rank const& data){
            data.printItSelf(os);
            return os;
        }
        
		virtual ~Rank(){};
};

class HITSRank : public Rank {
	public:
		HITSRank(double authRank, double hubRank) : _authorityRank(authRank), _hubRank(hubRank) {};
		~HITSRank(){};
		
		double authorityRank();
		double hubRank();
		void printItSelf(ostream& os) const;
		
	private:
		double _authorityRank;
		double _hubRank;
		
};

double HITSRank :: authorityRank(){ return _authorityRank; }
double HITSRank :: hubRank(){ return _hubRank; }
void HITSRank :: printItSelf(ostream& os) const {
    os << "Authority rank: " << _authorityRank << "; Hub rank: " << _hubRank << ";" << endl;
}

class PageRankInDegreeRank : public Rank {
	public:
		PageRankInDegreeRank(double rankValue) : _rankValue(rankValue) {};
		~PageRankInDegreeRank(){};
		
		double rankValue();
		void printItSelf(ostream& os) const;
		
	private:
		double _rankValue;
		
};

double PageRankInDegreeRank :: rankValue(){ return _rankValue; }
void PageRankInDegreeRank :: printItSelf(ostream& os) const{ os << _rankValue << endl; }

class WebPage {
    public:
        WebPage(int nodeId, list<int>* listOfLinkedWebPagesIds) : _id(nodeId), _listOfLinkedWebPagesIds(listOfLinkedWebPagesIds) {}
        ~WebPage();
        const char* PageName(){ return _name;}
        int pageId(){ return _id; }
        list<int>* listOfLinkedWebPagesIds(){ return _listOfLinkedWebPagesIds; }
        Rank* ranking(){ return _ranking; }
        void rankWebPage(Rank* ranking){ _ranking = ranking; }
        
    private:
        const char* _name;
        int _id;
        list<int>* _listOfLinkedWebPagesIds;
        Rank* _ranking;
};

WebPage :: ~WebPage(){
	delete _listOfLinkedWebPagesIds;
	delete _ranking;
}

#endif
