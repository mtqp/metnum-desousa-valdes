#ifndef __MAIN__
#define __MAIN__

#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include "ranking_algorithm.h"
#include "web_net.h"
#include "parsing.h"

using namespace std;

ParsingAlgorithm* CreateParsingAlgorithmFromParameter(InstanceType instanceType);
RankingAlgorithm* CreateRankingAlgorithmFromParameter(AlgorithmType algorithmType);

#endif
