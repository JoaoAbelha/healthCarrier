#include "GUI.h"
#include "Kosaraju.h"
#include "AStar.h"
#include "TSPNearestNeighbor.h"
#include "TwoOpt.h"
#include "CompleteGraph.h"
#include "mapParser.h"
#include "DFS.h"
#include <iostream>
#include <map>
#include <cmath>
#include <fstream>
#include <sstream>

float getDistanceBetween(pair<double, double> p1, pair<double, double> p2) {
	double xA = p1.first;
	double yA = p1.second;
	double xB = p2.first;
	double yB = p2.second;

	return sqrt((xB - xA) * (xB - xA) + (yB - yA) * (yB - yA));
}

bool GUI::is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

int GUI::checkInput(int min, int max, std::string error) {
	string input;
	std::cin >> input;
	int number;
	bool onlyNumber = is_number(input);
	if (onlyNumber)
		number = std::stoi(input);

	while (!std::cin.good() || !onlyNumber ||  number < min || number > max ) {
		std::cin.clear();
		std::cin.ignore(5000, '\n');
		std::cout << std::endl << error << std::endl;
		std::cin >> input;
		onlyNumber = is_number(input);
		if(onlyNumber)
			number = std::stoi(input);
	}
	std::cin.ignore(5000, '\n');
	return number;
}

void * GUI::loadGraph(void * fileName) {
        std::cout
    << " __________________________________________________________________________________ "
    << std::endl;
std::cout
    << "|                                                                                  |"
    << std::endl;
std::cout
    << "|                                CHOOSE THE MAP YOU WANT                           |"
    << std::endl;
std::cout
    << "|__________________________________________________________________________________|"
    << std::endl;

std::cout
<< "|                            1- Small Map                                          |"
<< std::endl;
std::cout
<< "|                            2- Medium Map                                         |"
<< std::endl;
std::cout
<< "|                            3- Big Map                                            |"
<< std::endl;
std::cout
<< "|__________________________________________________________________________________| "<<std::endl;

std::cout << "  0 - Back to main menu" << std::endl << std::endl<<std::endl;

    int result = checkInput(0, 3, "Choose one of the given maps");
    Graph<int> * graph;
    switch (result) {
    case 0:
        return NULL;
    case 1:
        graph = parseMap("maps/map_small_A.txt", "maps/map_small_B.txt",
                "maps/map_small_C.txt");
        break;
    case 2:
        graph = parseMap("maps/map_medium_A.txt", "maps/map_medium_B.txt",
                "maps/map_medium_C.txt");
        break;
    case 3:
        graph = parseMap("maps/map_big_A.txt", "maps/map_big_B.txt",
                "maps/map_big_C.txt");
        break;

    }
    graph->displayOnGraphViewer(600, 600, false, "blue", "black");
    return graph;
}

GUI::GUI(std::string title) {
	this->title = title;
}

Graph<int> GUI::getMap() {
	return map;
}

void GUI::displayAllMenuOPtions(
		std::vector<std::pair<std::string, voidFunctionType>> items) {
	int i = 1;
	for (const auto &p : items) {
		std::cout << i++ << "-\t" << p.first << std::endl;
	}
	std::cout << "0- \tGo back \n" << std::endl;
}


void GUI::displayMainMenuOptions() {
	std::cout << title << std::endl;
	std::cout
			<< " __________________________________________________________________________________ "
			<< std::endl;
	std::cout
			<< "|                                                                                  |"
			<< std::endl;
	std::cout
			<< "|                                 W E L C O M E !                                  |"
			<< std::endl;
	std::cout
			<< "|__________________________________________________________________________________|"
			<< std::endl;
	std::cout
			<< "|                                                                                  |"
			<< std::endl;
	std::cout
			<< "|  Opcoes disponiveis:                                                             |"
			<< std::endl;
	std::cout
			<< "|                                                                                  |"
			<< std::endl;
	std::cout
			<< "|      1 - Visualizar grafo / Grafo pre processado                                 |"
			<< std::endl;
	std::cout
			<< "|                                                                                  |"
			<< std::endl;
	std::cout
			<< "|      2 - Verificar se o grafo apresenta uma boa representacao de um mapa real    |"
			<< std::endl;
	std::cout
			<< "|                                                                                  |"
			<< std::endl;
	std::cout
			<< "|      3- Possibilidade de transporte                                              |"
			<< std::endl;
	std::cout
			<< "|                                                                                  |"
			<< std::endl;
	std::cout
			<< "|      4- Melhor caminho para ambulancia de capacidade finita                      |"
			<< std::endl;
	std::cout
			<< "|                                                                                  |"
			<< std::endl;
	std::cout
			<< "|      5- Caminho otimo considerando varias ambulancias de capacidade limitada     |"
			<< std::endl;
	std::cout
			<< "|                                                                                  |"
			<< std::endl;
	std::cout
			<< "|__________________________________________________________________________________|"
			<< std::endl;
	std::cout
			<< "|                                                                                  |"
			<< std::endl;
	std::cout
			<< "|  Test files:                                                                     |"
			<< std::endl;
	std::cout
			<< "|                                                                                  |"
			<< std::endl;
	std::cout
			<< "|                                                                                  |"
			<< std::endl;
	std::cout
			<< "|__________________________________________________________________________________|"
			<< std::endl;
	std::cout << std::endl;
}

void GUI::UIConnected(bool isSConnected) {
	if (isSConnected)
		cout << "Represents a real graph" << endl;
	else
		cout << "Cannot represent a real graph" << endl;

}

void GUI::createMenu(std::string title,
		std::vector<std::pair<std::string, voidFunctionType>> items) {

	std::string error = "Invalid Input";
	while (1) {
		std::cout << title << std::endl;
		std::cout << "Choose a number within the range" << std::endl;
		displayAllMenuOPtions(items);
		int select = checkInput(0, items.size(), error);

		if (select == 0)
			break;
		else {
			voidFunctionType type = items[select - 1].second;
			Graph<int> * returnGraph = (Graph<int> *) (this->*type)(NULL);
			if (returnGraph == nullptr)
				continue;
			map = *returnGraph;
		}
	}
	std::cout << "Exiting..." << std::endl;
}

void * GUI::checkConnectivity(void * arg) {
	Kosaraju<int> k(map);
	UIConnected(k.algorithm());
	return NULL;
}

void * GUI::calcAStar(void * arg) {
	int startNodeId; // = 2557;
	int finishNodeId; // = 2536;

	std::cout << "Input start node ID" << std::endl;
	std::cin >> startNodeId;
	while (!cin
			|| (startNodeId <= 0 || startNodeId >= map.getVertexSet().size())) {
		std::cout << "Invalid ID" << std::endl;
		std::cout << "Input start node ID" << std::endl;
		std::cin >> startNodeId;
	}

	std::cout << "Input hospital node ID" << std::endl;
	std::cin >> finishNodeId;
	while (!cin
			|| (finishNodeId <= 0 || finishNodeId >= map.getVertexSet().size())) {
		std::cout << "Invalid ID" << std::endl;
		std::cout << "Input hospital node ID" << std::endl;
		std::cin >> finishNodeId;
	}

	AStar<int> a = AStar<int>(map);
	DFS<int> dfs(map);


	vector<int> result;
	if (dfs.checkPath(startNodeId, finishNodeId)) {
		result = a.calcOptimalPath(startNodeId, finishNodeId);

		if(!a.foundSolution()) {
			std::cout << finishNodeId << " is unreachable from " << startNodeId << std::endl<< std::endl<< std::endl;
			return nullptr;
		}
	}
	else {
		std::cout << finishNodeId << " is unreachable from " << startNodeId << std::endl<< std::endl<< std::endl;
		return nullptr;
	}


	map.showShortestPath(result,600, 600, false, "blue", "black");


	return nullptr;
}

void * GUI::calcNearestNeighbor(void * arg) {
	int startNodeId; // = 2557;
	int finishNodeId; // = 2536;

	std::cout << "Input start node ID" << std::endl;
	std::cin >> startNodeId;
	while (!cin
			|| (startNodeId <= 0 || startNodeId >= map.getVertexSet().size())) {
		std::cout << "Invalid ID" << std::endl;
		std::cout << "Input start node ID" << std::endl;
		std::cin >> startNodeId;
	}

	std::cout << "Input hospital node ID" << std::endl;
	std::cin >> finishNodeId;
	while (!cin
			|| (finishNodeId <= 0 || finishNodeId >= map.getVertexSet().size())) {
		std::cout << "Invalid ID" << std::endl;
		std::cout << "Input hospital node ID" << std::endl;
		std::cin >> finishNodeId;
	}

	vector<int> pois;
	int healthCenter;
	string healthCenterString;
	string poisString;
	bool inputFlag = true;
	while (inputFlag) {
		std::cout
				<< "Input health center node IDs (1 line separeted with spaces)"
				<< std::endl;
		std::getline(std::cin, poisString);
		istringstream poisSS(poisString);
		while (std::getline(poisSS, healthCenterString, ' ')) {
			if (is_number(healthCenterString)) {
				healthCenter = stoi(healthCenterString);
				pois.push_back(healthCenter);
				inputFlag = false;
			}
			else {
				inputFlag = true;
				break;
			}

		}
	}

	TSPNearestNeighbor<int> nn(map);
	DFS<int> dfs(map);
	vector<Vertex<int>> dfsVector = dfs.algorithm(Vertex<int>(startNodeId));

	int poiCounter = 0;
	for (int p : pois) {
		for (auto v : dfsVector) {
			if (p == v.getInfo()) {
				poiCounter++;
				break;
			}
		}
	}
	if (poiCounter != pois.size()) {
		std::cout << "Couldnt reach all health centers" << std::endl;
		return nullptr;
	}

	vector<int> result;
	if (dfs.checkPath(startNodeId, finishNodeId)) {
		result = nn.calcPath(startNodeId, finishNodeId, pois);
	}
	else {
		std::cout << finishNodeId << " is unreachable from " << startNodeId << std::endl;
		return nullptr;
	}

	map.showShortestPath(result,600, 600, false, "blue", "black");

	if (result.size() > 0) {
		std::cout << "Path: ";
		for (unsigned int i = 0; i < result.size(); i++) {
			std::cout << result[i] << " -> ";
		}
	}

	return nullptr;
}

void * GUI::calcOneAmbulanceMultipleHealthCenters(void * arg) {
	int startNodeId;
	int finishNodeId;

	std::cout << "Input start node ID" << std::endl;
	std::cin >> startNodeId;
	while (!cin
			|| (startNodeId <= 0 || startNodeId >= map.getVertexSet().size())) {
		std::cout << "Input start node ID" << std::endl;
		std::cin >> startNodeId;
	}

	std::cout << "Input hospital node ID" << std::endl;
	std::cin >> finishNodeId;
	while (!cin
			|| (finishNodeId <= 0 || finishNodeId >= map.getVertexSet().size())) {
		std::cout << "Input hospital node ID" << std::endl;
		std::cin >> finishNodeId;
	}

	vector<int> pois;
	int healthCenter;
	string healthCenterString;
	string poisString;
	bool inputFlag = true;
	while (inputFlag) {
		std::getline(std::cin, poisString);
		std::cout
				<< "Input health center node IDs (1 line separeted with spaces)"
				<< std::endl;
		std::getline(std::cin, poisString);
		istringstream poisSS(poisString);
		while (std::getline(poisSS, healthCenterString, ' ')) {
			if (is_number(healthCenterString)) {
				healthCenter = stoi(healthCenterString);
				pois.push_back(healthCenter);
				inputFlag = false;
			}
			else {
				inputFlag = true;
				break;
			}

		}
	}

	vector<int> result;

	CompleteGraph<int> cg;

	cg.graph = &map;

	for (int id : pois) {
		HealthCenter<int> * hc = new HealthCenter<int>(id);
		cg.addHealthCenter(hc);
	}

	Ambulance<int> * a = new Ambulance<int>(map.getVertex(startNodeId), UINT_MAX);
	a->setPickUp(pois);
	Hospital<int> * h = new Hospital<int>(Vertex<int>(finishNodeId));
	result = cg.generateOneAmbulanceMultipleHealthCenter(a, h);

	map.showShortestPath(result,600, 600, false, "blue", "black");

	for (unsigned int i = 0; i < result.size(); i++) {
		std::cout << result[i] << " -> ";
	}

	return nullptr;
}

void * GUI::calcMultipleAmbulanceMultipleHealthCenters(void * arg) {
	int startNodeId;
	int finishNodeId;

	std::cout << "Input start node ID" << std::endl;
	std::cin >> startNodeId;
	while (!cin
			|| (startNodeId <= 0 || startNodeId >= map.getVertexSet().size())) {
		std::cout << "Input start node ID" << std::endl;
		std::cin >> startNodeId;
	}

	std::cout << "Input hospital node ID" << std::endl;
	std::cin >> finishNodeId;
	while (!cin
			|| (finishNodeId <= 0 || finishNodeId >= map.getVertexSet().size())) {
		std::cout << "Input hospital node ID" << std::endl;
		std::cin >> finishNodeId;
	}

	vector<int> pois;
	int healthCenter;
	string healthCenterString;
	string poisString;
	bool inputFlag = true;
	while (inputFlag) {
		std::getline(std::cin, poisString);
		std::cout
				<< "Input health center node IDs (1 line separeted with spaces)"
				<< std::endl;
		std::getline(std::cin, poisString);
		istringstream poisSS(poisString);
		while (std::getline(poisSS, healthCenterString, ' ')) {
			if (is_number(healthCenterString)) {
				healthCenter = stoi(healthCenterString);
				pois.push_back(healthCenter);
				inputFlag = false;
			}
			else {
				inputFlag = true;
				break;
			}

		}
	}

	vector<vector<int>> result;

	CompleteGraph<int> cg;

	cg.graph = &map;

	for (int i = 0; i < pois.size(); i++) {
		HealthCenter<int>* hc = new HealthCenter<int>(pois[i]);
		cg.addHealthCenter(hc);
	}

	for (auto & healthCenter : cg.getHealthCenter()) {
		int patientsNo = rand() % 10 + 1;
		for (int i = 0; i < patientsNo; i++) {
			healthCenter->addPatient(new Patient());
		}
	}

	Hospital<int> * h = new Hospital<int>(Vertex<int>(finishNodeId));
	result = cg.generateMultipleAmbulancesMultipleHealthCenter(h,
			new Vertex<int>(startNodeId),pois);


	for (unsigned int j = 0; j < result.size(); j++) {
		vector<int> subresult = result[j];
		map.showShortestPath(subresult,600, 600, false, "blue", "black");
		for (unsigned int i = 0; i < result[j].size(); i++) {
			std::cout << subresult[i] << " -> ";
		}
		std::cout << std::endl;
	}

	return nullptr;
}

void * GUI::calcMultipleAmbulanceMultipleHealthCentersWithRestrictions(void * arg) {
	int startNodeId;
		int finishNodeId;

		std::cout << "Input start node ID" << std::endl;
		std::cin >> startNodeId;
		while (!cin
				|| (startNodeId <= 0 || startNodeId >= map.getVertexSet().size())) {
			std::cout << "Input start node ID" << std::endl;
			std::cin >> startNodeId;
		}

		std::cout << "Input hospital node ID" << std::endl;
		std::cin >> finishNodeId;
		while (!cin
				|| (finishNodeId <= 0 || finishNodeId >= map.getVertexSet().size())) {
			std::cout << "Input hospital node ID" << std::endl;
			std::cin >> finishNodeId;
		}

		vector<int> pois;
		int healthCenter;
		string healthCenterString;
		string poisString;
		bool inputFlag = true;
		while (inputFlag) {
			std::getline(std::cin, poisString);
			std::cout
					<< "Input health center node IDs (1 line separeted with spaces)"
					<< std::endl;
			std::getline(std::cin, poisString);
			istringstream poisSS(poisString);
			while (std::getline(poisSS, healthCenterString, ' ')) {
				if (is_number(healthCenterString)) {
					healthCenter = stoi(healthCenterString);
					pois.push_back(healthCenter);
					inputFlag = false;
				}
				else {
					inputFlag = true;
					break;
				}

			}
		}


	vector<vector<int>> result;

	CompleteGraph<int> cg;
	cg.graphOriginal = &map;

	for (int i = 0 ; i < pois.size(); i++) {
		HealthCenter<int>* hc = new HealthCenter<int>(pois[i]);
		cg.addHealthCenter(hc);
	}


	vector<Hospital<int>*> hospitals;

	result = cg.generateMultipleAmbulancesMultipleHealthCenterWithRestrictions(h, new Vertex<int>(2557));

	Graph<int> * graph = new Graph<int>();

	for (unsigned int j = 0; j < result.size(); j++) {
		vector<int> subresult = result[j];
		map.showShortestPath(subresult,600, 600, false, "blue", "black");
	}

	return graph;
}

void * GUI::preProcess(void * arg) {
	map.preProcessGraph();
	map.displayOnGraphViewer(600, 600, false, "blue", "black");
	return &map;
}

void GUI::createMainMenuOptions(std::string menu[]) {
	std::vector<std::pair<std::string, voidFunctionType>> items;

	items.push_back(
			std::pair<std::string, voidFunctionType>(menu[0], loadGraph));
	items.push_back(
			std::pair<std::string, voidFunctionType>(menu[1],
					checkConnectivity));
	items.push_back(
			std::pair<std::string, voidFunctionType>(menu[2], calcAStar));
	items.push_back(
			std::pair<std::string, voidFunctionType>(menu[3],
					calcNearestNeighbor));
	items.push_back(
			std::pair<std::string, voidFunctionType>(menu[4], preProcess));
	items.push_back(
			std::pair<std::string, voidFunctionType>(menu[5],
					calcOneAmbulanceMultipleHealthCenters));
	items.push_back(
						std::pair<std::string, voidFunctionType>(menu[6], calcMultipleAmbulanceMultipleHealthCenters));
	items.push_back(
							std::pair<std::string, voidFunctionType>(menu[7], calcMultipleAmbulanceMultipleHealthCenters));

	createMenu("Main menu", items);
}
