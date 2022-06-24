#ifndef CAL_MAPPARSER_H
#define CAL_MAPPARSER_H

#include <iostream>
#include <fstream>
#include <map>

#include "Graph.h"

Graph<int> *parseMap(std::string filePathA, std::string filePathB, std::string filePathC);

#endif //CAL_MAPPARSER_H
