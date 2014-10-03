#include "web_page.h"

using namespace std;

double HITSRank :: authorityRank(){ return _authorityRank; }

double HITSRank :: hubRank(){ return _hubRank; }

void HITSRank :: printItSelf(ostream& os) const {
    os << "Authority rank: " << _authorityRank << "; Hub rank: " << _hubRank << ";" << endl;
}

double PageRankInDegreeRank :: rankValue(){ return _rankValue; }
void PageRankInDegreeRank :: printItSelf(ostream& os) const{ os << _rankValue << endl; }

WebPage :: ~WebPage(){
	delete _listOfLinkedWebPagesIds;
	delete _ranking;
}

const char* WebPage :: PageName(){ return _name;}

int WebPage :: pageId(){ return _id; }

list<int>* WebPage :: listOfLinkedWebPagesIds(){ return _listOfLinkedWebPagesIds; }

Rank* WebPage :: ranking(){ return _ranking; }

void WebPage :: addToLinkedIdList(int linkedId){ _listOfLinkedWebPagesIds->push_back(linkedId); }

void WebPage :: rankWebPage(Rank* ranking){ _ranking = ranking; }
