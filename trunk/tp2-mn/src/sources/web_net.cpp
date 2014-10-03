#include "web_net.h"

list<WebPage*>* WebNet :: webPages(){ return _webPages; }

int WebNet :: amountOfNodes(){ return _amountOfNodes; }

int WebNet :: amountOfEdges(){ return _amountOfEdges; }

WebNet :: ~WebNet(){
	for (list<WebPage*>::iterator it = _webPages->begin(); it != _webPages->end(); ++it){
		delete *it;
	}
	delete _webPages;
}
