#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include "web_page.cpp"
//TODO DELETE WHAT WE ARE NOT USING

//TODO: REFACTOR INTO MULTIPLE FILES!

using namespace std;

#ifndef __WEB_NET__
#define __WEB_NET__

class WebNet{
	public:
		WebNet(int amountOfNodes, int amountOfEdges, list<WebPage*>* webPages) : _amountOfNodes(amountOfNodes), _amountOfEdges(amountOfEdges), _webPages(webPages) {}; 
		~WebNet();
		list<WebPage*>* webPages();
		int amountOfNodes();
		int amountOfEdges();
	
	private: 
		int _amountOfNodes;
		int _amountOfEdges;
		list<WebPage*>* _webPages; 
};

list<WebPage*>* WebNet :: webPages(){ return _webPages; }
int WebNet :: amountOfNodes(){ return _amountOfNodes; }
int WebNet :: amountOfEdges(){ return _amountOfEdges; }


WebNet :: ~WebNet(){
	for (list<WebPage*>::iterator it = _webPages->begin(); it != _webPages->end(); ++it){
		delete *it;
	}
	delete _webPages;
}

#endif
