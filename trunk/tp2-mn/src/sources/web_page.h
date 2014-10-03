#ifndef __WEB_PAGE__
#define __WEB_PAGE__

#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;

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

class PageRankInDegreeRank : public Rank {
	public:
		PageRankInDegreeRank(double rankValue) : _rankValue(rankValue) {};
		~PageRankInDegreeRank(){};
		
		double rankValue();
		void printItSelf(ostream& os) const;
		
	private:
		double _rankValue;
		
};

class WebPage {
    public:
        WebPage(int nodeId, list<int>* listOfLinkedWebPagesIds) : _id(nodeId), _listOfLinkedWebPagesIds(listOfLinkedWebPagesIds) {}
        ~WebPage();
        
        const char* PageName();
        int pageId();
        list<int>* listOfLinkedWebPagesIds();
        Rank* ranking();
        void addToLinkedIdList(int linkedId);
        void rankWebPage(Rank* ranking);
        
    private:
        const char* _name;
        int _id;
        list<int>* _listOfLinkedWebPagesIds;
        Rank* _ranking;
};

#endif
