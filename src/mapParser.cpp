#include "mapParser.h"
#include "Graph.h"
#include <cmath>
#include <map>

static void parseFileA(std::ifstream & fileA, Graph<int> * graph, std::map<unsigned long long, int> & ids);
static void parseFileB(std::ifstream & fileB, std::map<unsigned long long, std::pair<std::string, bool> > & edges);
static void parseFileC(std::ifstream & fileC, Graph<int> * graph, const std::map<unsigned long long,int> & id, const std::map<unsigned long long, std::pair<std::string, bool>>& edge);

Graph<int> *parseMap(std::string filePathA, std::string filePathB, std::string filePathC){
	Graph<int> * graph = new Graph<int>();

    std::ifstream fileA(filePathA);
    std::ifstream fileB(filePathB);
    std::ifstream fileC(filePathC);

    std::map<unsigned long long, int> id;
    parseFileA(fileA, graph, id);
    fileA.close();

    std::map<unsigned long long, std::pair<std::string, bool>> edges;
    parseFileB(fileB, edges);
    fileB.close();

    //Add edges, complementing information with the one at edgeMap
    parseFileC(fileC, graph, id, edges);
    fileC.close();


    return graph;
}


static void parseFileA(std::ifstream & fileA, Graph<int> *graph, std::map<unsigned long long, int> &id) {
    std::string currLine = "";

    int counter = 1;

    while(getline(fileA, currLine)) {
        unsigned long long vertexInfo = stoull(currLine.substr(0, currLine.find_first_of(";")));
        currLine = currLine.substr(currLine.find_first_of(";") + 1);

        double latRad = stod(currLine.substr(currLine.find_last_of(";") + 1));
        currLine = currLine.substr(0, currLine.find_last_of(";"));

        double longRad = stod(currLine.substr(currLine.find_last_of(";") + 1));

        id.insert(std::make_pair(vertexInfo, counter));

        double EARTH_RADIUS = 6371000;
        double x = EARTH_RADIUS * longRad * cos(latRad);
        double y = EARTH_RADIUS * latRad;

        graph->addVertex(counter);
        graph->getVertex(counter)->setX(x);
        graph->getVertex(counter++)->setY(y);

    }

}



static void parseFileB(std::ifstream &fileB, std::map<unsigned long long, std::pair<std::string, bool> > &edges) {
    std::string currLine = "";

    while(!fileB.eof()) {
        getline(fileB, currLine);

        unsigned long long edgeID = stoull(currLine.substr(0, currLine.find_first_of(";")));
        currLine = currLine.substr(currLine.find_first_of(";") + 1);
        std::string edgeLabel = (currLine[0] == ';') ?  "" : currLine.substr(0, currLine.find_first_of(";"));
        currLine = currLine.substr(currLine.find_first_of(";") + 1);
        bool isTwoWay = (currLine == "True");

        edges.insert(std::make_pair(edgeID, std::make_pair(edgeLabel, isTwoWay)));
    }
}

static void parseFileC(std::ifstream & fileC, Graph<int> *graph, const std::map<unsigned long long,int> & id, const std::map<unsigned long long, std::pair<std::string, bool>>& edges) {
    std::string currLine = "";

    while(!fileC.eof()) {
        getline(fileC, currLine);

        unsigned long long edgeID = stoull(currLine.substr(0, currLine.find_first_of(";")));
        currLine = currLine.substr(currLine.find_first_of(";") + 1);

        unsigned long long startVertex = stoull(currLine.substr(0, currLine.find_first_of(";")));
        currLine = currLine.substr(currLine.find_first_of(";") + 1);

        unsigned long long finalVertex = stoull(currLine);

        double edgeWeight = graph->getVertex(id.at(startVertex))->getDistanceToOtherVertex(*graph->getVertex(id.at(finalVertex)));

        std::string edgeLabel = edges.at(edgeID).first;
        bool edgeIsBothWays = edges.at(edgeID).second;


        graph->addEdge(graph->getVertex(id.at(startVertex)),graph->getVertex(id.at(finalVertex)), edgeWeight, edgeIsBothWays);
    }
}
