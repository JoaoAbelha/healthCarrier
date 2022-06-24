#ifndef SRC_GUI_H_
#define SRC_GUI_H_
#include <string>
#include <vector>
#include <utility>
#include <chrono>
#include "Graph.h"

class GUI;

typedef void * (GUI::*voidFunctionType)(void *);

class GUI {
private:
	Graph<int> map;
	std::string title;
	void displayAllMenuOPtions(std::vector<std::pair<std::string, voidFunctionType>> items);
	void UIConnected(bool isSConnected);

public:
	GUI(std::string title);
	Graph<int> getMap();
	bool is_number(const std::string& s);
	void displayMainMenuOptions();
	void createMainMenuOptions(std::string menu[]);
	void createMenu(std::string title,std::vector<std::pair<std::string, voidFunctionType>> items);
	int checkInput(int min, int max, std::string error);
	void * loadGraph(void * fileName);
	void * checkConnectivity(void * arg);
	void * calc2opt(void * arg);
	void * calcAStar(void * arg);
	void * calcNearestNeighbor(void * arg);
	void * calcOneAmbulanceMultipleHealthCenters(void * arg);
	void * calcMultipleAmbulanceMultipleHealthCenters(void * arg);
	void * calcMultipleAmbulanceMultipleHealthCentersWithRestrictions(void * arg);
	void * preProcess(void * arg);

};



#endif /* SRC_GUI_H_ */
