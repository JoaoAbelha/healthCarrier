#ifndef CAL_TSPNEARESTNEIGHBOR_H
#define CAL_TSPNEARESTNEIGHBOR_H

#include <unordered_set>
#include <list>
#include "DFS.h"
#include "AStar.h"

using namespace std;

#define INFINITE_DOUBLE std::numeric_limits<double>::max()

template<class T>
class TSPNearestNeighbor {
private:
	// Data Structures
	Graph<T> map;
	DFS<T> dfs;
	VertexHashTable<T> pois;

	vector<T> visitOrder;
	vector<T> lastSolution;

	// Variables for current calculation
	Vertex<T> finishNode;
	Vertex<T> startNode;
	double solutionTotalCost = INFINITE_DOUBLE;
	unsigned int visitOrderFinalSize = 0;

	// Verifies if nodes to visit are valid : throws InvalidNodeId(id) if invalid node is found!
	void verifyValidNodes(const vector<T> & pointsOfInterest);

	// Returns the closest node to 'node' in the 'otherNodes' hash table
	Vertex<T> getClosestNode(const Vertex<T> & node, const VertexHashTable<T> otherNodes) const;

	// Adds 'node' to the visit order
	void addToVisitOrder(const Vertex<T> & node);

	// Removes 'node' from the visit order
	void removeFromVisitOrder(const Vertex<T> & node);

	// Builds the path by performing A-Star algorithm between the nodes in the visitOrder, in the corret order
	void buildSolution();

	// Appends two vectors
	void append_vector(vector<T> & v1, const vector<T> & v2);

	// Initializes Data Structures for algorithm to operate
	void initDataStructures();

public:
	TSPNearestNeighbor(const Graph<T> &graph);

	// Finds the best visit order between start and finish, placing that order in the 'visitOrder' list
	void findBestVisitOrder(const Vertex<T> & start, const Vertex<T> & finish);

	// Calculates path between two nodes, passing through all the points of interest
	vector<T> calcPath(int startNodeId, int finishNodeId, const vector<T> & pois);

	// Calculates path between two nodes, passing through all the points of interest
	vector<T> calcPathWithCondition(int startNodeId, int finishNodeId, const vector<T> & pointsOfInterest);

	// Returns a vector with the Points of Interest Visit Order
	vector<T> getVisitOrder() const;

	// Returns the solution weight, if there is a solution at the present moment
	double getSolutionWeight() const;
};

template<class T>
TSPNearestNeighbor<T>::TSPNearestNeighbor(const Graph<T> &graph) :
		map(graph), dfs() {
	dfs.setGraph(map);
}

template<class T>
vector<T> TSPNearestNeighbor<T>::calcPath(int startNodeId, int finishNodeId,
		const vector<T> & pointsOfInterest) {

	initDataStructures();

	startNode = *map.getVertex(startNodeId);
	finishNode = *map.getVertex(finishNodeId);
	verifyValidNodes(pointsOfInterest);

	// When the visit order list is complete it should have the startNodeId + all the POIS ids in the correct order + finishNodeId
	visitOrderFinalSize = 2 + pois.size();

	findBestVisitOrder(startNode, finishNode);

	visitOrder.push_back(finishNodeId);

	// There was no possible path.
	if (visitOrder.size() != visitOrderFinalSize) {
		lastSolution.clear();
		std::cout << "Impossible Path" << std::endl;
		return lastSolution;
	} else {
		buildSolution();
		return lastSolution;
	}
}

template<class T>
vector<T> TSPNearestNeighbor<T>::calcPathWithCondition(int startNodeId,
		int finishNodeId, const vector<T> & pointsOfInterest) {
	initDataStructures();

	startNode = *map.getVertex(startNodeId);
	finishNode = *map.getVertex(finishNodeId);
	verifyValidNodes(pointsOfInterest);

	// When the visit order list is complete it should have the startNodeId + all the POIS ids in the correct order + finishNodeId
	visitOrderFinalSize = 2 + pois.size();

	findBestVisitOrder(startNode, finishNode);

	visitOrder.push_back(finishNodeId);

	// There was no possible path.
	if (visitOrder.size() != visitOrderFinalSize) {
		lastSolution.clear();
		std::cout << "BAD PATH" << std::endl;
		return lastSolution;
	} else {

		buildSolution();
		return lastSolution;
	}
}

template<class T>
double TSPNearestNeighbor<T>::getSolutionWeight() const {
	return solutionTotalCost;
}

template<class T>
void TSPNearestNeighbor<T>::initDataStructures() {
	visitOrder.clear();
	lastSolution.clear();
	pois.clear();
}

template<class T>
void TSPNearestNeighbor<T>::verifyValidNodes(
		const vector<T> & pointsOfInterest) {
	for (T id : pointsOfInterest) {
		if (id != startNode.getInfo() && id != finishNode.getInfo()) {
			pois.insert(*map.getVertex(id));
		}
	}
}

template<class T>
void TSPNearestNeighbor<T>::findBestVisitOrder(const Vertex<T> & start,
		const Vertex<T> & finish) {

	// All are reachable from this node -> Add it to the visit order list and removed it from the POIs hash table
	addToVisitOrder(start);

	Vertex<T> closestNode;
	VertexHashTable<T> poisToVisit = pois;
	while (!poisToVisit.empty()) {
		closestNode = getClosestNode(startNode, poisToVisit);

		findBestVisitOrder(closestNode, finishNode);

		// If the list is not complete, the current order does not provide a possible path
		if (visitOrder.size() != visitOrderFinalSize - 1) {
			poisToVisit.erase(closestNode);
		} else {
			return;
		}
	}

	// Perform back-tracking if solution wasn't achieved
	if (visitOrder.size() != visitOrderFinalSize - 1) {
		removeFromVisitOrder(start);
	}

	return;
}

template<class T>
void TSPNearestNeighbor<T>::addToVisitOrder(const Vertex<T> & node) {
	visitOrder.push_back(node.getInfo());
	pois.erase(node);
}

template<class T>
void TSPNearestNeighbor<T>::removeFromVisitOrder(const Vertex<T> & node) {
	visitOrder.pop_back();
	pois.insert(node);
}

template<class T>
Vertex<T> TSPNearestNeighbor<T>::getClosestNode(const Vertex<T> & node,
		const VertexHashTable<T> otherNodes) const {
	Vertex<T> closestNode = *(otherNodes.begin());

	double closestNodeDistance = node.getDistanceToOtherVertex(closestNode);
	double dist;

	for (Vertex<T> n : otherNodes) {
		dist = node.getDistanceToOtherVertex(n);
		if (dist < closestNodeDistance) {
			closestNode = n;
			closestNodeDistance = dist;
		}
	}

	return closestNode;
}

template<class T>
void TSPNearestNeighbor<T>::buildSolution() {
	AStar<T> astar(map);
	solutionTotalCost = 0;

	for (int i = 0; i < visitOrder.size() - 1; i++) {
		append_vector(lastSolution,
				astar.calcOptimalPath(visitOrder.at(i), visitOrder.at(i + 1)));

		solutionTotalCost += astar.getSolutionWeight();

		if (i != visitOrder.size() - 2) {
			lastSolution.pop_back();
		}
	}
}

template<class T>
void TSPNearestNeighbor<T>::append_vector(vector<T> & v1,
		const vector<T> & v2) {
	for (int i : v2) {
		v1.push_back(i);
	}
}

template<class T>
vector<T> TSPNearestNeighbor<T>::getVisitOrder() const {
	return this->visitOrder;
}

#endif //CAL_TSPNEARESTNEIGHBOR_H
