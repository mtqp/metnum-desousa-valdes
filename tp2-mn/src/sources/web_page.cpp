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
	public:
		Rank(){};
		~Rank(){};
};

class WebPage {
    public:
        WebPage(int nodeId, list<int>* listOfLinkedWebPagesIds) : _id(nodeId), _listOfLinkedWebPagesIds(listOfLinkedWebPagesIds) {}
        ~WebPage();
        const char* PageName(){ return _name;}
        
    private:
        const char* _name;
        int _id;
        list<int>* _listOfLinkedWebPagesIds;
        Rank _ranking;
};

WebPage :: ~WebPage(){
	delete _listOfLinkedWebPagesIds;
}

#endif