#ifndef __WEB_NET__
#define __WEB_NET__

#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include "web_page.h"

using namespace std;

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

#endif
