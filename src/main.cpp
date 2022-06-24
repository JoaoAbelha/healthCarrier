#include <fstream>
#include <sstream>
#include <string>
#include "GraphViewer/cpp/graphviewer.h"
#include "Graph.h"
#include <map>
#include <cmath>
#include "Kosaraju.h"
#include "GUI.h"


int main() {
	GUI view("Rossetti foreva");
	std::string menu[] = {"Load graph", "Check Connectivity", "A*", "NearestNeighbor", "Preprocess Graph", "One Ambulance, Multiple HealthCenters", "Multiple Ambulance, Multiple HealthCenters", "Multiple Ambulance, Multiple HealthCenters, Disease Types"};
	view.createMainMenuOptions(menu);
}
