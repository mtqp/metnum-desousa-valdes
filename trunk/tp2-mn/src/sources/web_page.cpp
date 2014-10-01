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
        
		~Rank(){};
};

class HITSRank : public Rank {
	public:
		HITSRank(double authRank, double hubRank) : _authorityRank(authRank), _hubRank(hubRank) {};
		~HITSRank(){};
		
		double authorityRank(){ return _authorityRank; }
		double hubRank(){ return _hubRank; }
		void printItSelf(ostream& os) const{
			os << "Authority rank: " << _authorityRank << "; Hub rank: " << _hubRank << ";" << endl;
		}
		
	private:
		double _authorityRank;
		double _hubRank;
		
};

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
