#ifndef CAL_ASTAR_H
#define CAL_ASTAR_H

#include <set>
#include <float.h>
#include "Graph.h"
#include "AStarVertex.h"

template<class T>
class AStar {
protected:
    const Graph<T> &graph;
    AStarVertex<T> finishVertex;
    AStarVertex<T> startVertex;
    double solutionTotalCost = DBL_MAX;
    vector<T> lastSolution;
    set<AStarVertex<T>> pQueueWeighted; // stores the Vertexes with the A* weight function
    AStarVertexHashTable<T> checkedVertexes;
    vector<double> distancesToFinish;
    AStarVertex<T> topVertex;

    void initDataStructures();
    void populateQueue();
    bool foundOptimalSolution();
    bool queueIsEmpty() const;
    void removeVertexFromQueue();
    void updateTopVertex();
    void updateVertexOnQueue(const AStarVertex<T> &currVertex,  set<AStarVertex<T>> &queue);
    void updateQueue();
    void buildPath();
public:
    vector<T> calcOptimalPath(T startVertexId, T finishVertexId);
    AStar(const Graph<T> &graph);
    double getSolutionWeight() const;
    bool foundSolution() const;
};

template<class T>
AStar<T>::AStar(const Graph<T> &graph) : graph(graph) , topVertex(0) {}

template<class T>
double AStar<T>::getSolutionWeight() const{
    return solutionTotalCost;
}

template<class T>
bool AStar<T>::foundSolution() const {
    return !(checkedVertexes.empty() || checkedVertexes.find(finishVertex.getInfo())->getTotalWeight() == DBL_MAX);
}

template<class T>
void  AStar<T>::initDataStructures() {
    checkedVertexes.clear();
    lastSolution.clear();
    populateQueue();
}

template<class T>
vector<T> AStar<T>::calcOptimalPath(T startVertexId, T finishVertexId) {

    startVertex = *(graph.getVertex(startVertexId));
    finishVertex = *(graph.getVertex(finishVertexId));

    initDataStructures();

    while (!pQueueWeighted.empty()) {
        topVertex = *(pQueueWeighted.begin());

        if(foundOptimalSolution()) {
            buildPath();
            break;
        }

        if(topVertex.getAdj().empty()) {
            pQueueWeighted.erase(topVertex);
            continue;
        }

        updateQueue();
    }

    return lastSolution;
}

template<class T>
void AStar<T>::populateQueue() {
    pQueueWeighted.clear();

    Vertex<T> *vertex;

    for (auto & vertex : graph.getVertexSet()) {
    	distancesToFinish.push_back(vertex->getDistanceToOtherVertex(this->finishVertex));

        if(vertex->getInfo() == this->startVertex.getInfo()) {

            pQueueWeighted.emplace(*vertex, 0, vertex->getDistanceToOtherVertex(this->finishVertex));
        } else {
            pQueueWeighted.emplace(*vertex);
        }
    }
}

template<class T>
void AStar<T>::updateQueue(){
    pQueueWeighted.erase(pQueueWeighted.begin());
    checkedVertexes.insert(AStarVertex<T>(topVertex));

    vector<Edge<T>> edges = this->topVertex.getAdj();

    for (Edge<T> e : edges) {
        if (checkedVertexes.find(e.getDest()->getInfo()) != checkedVertexes.end()) {
            continue;
        }

        AStarVertex<T> curr(*(graph.getVertex(e.getDest()->getInfo())), topVertex.getInfo(), topVertex.getTotalWeight() + e.getWeight(), distancesToFinish.at(e.getDest()->posAtVec-1));
        updateVertexOnQueue(curr, pQueueWeighted);
    }
}

template<class T>
bool AStar<T>::foundOptimalSolution() {
    return (this->topVertex.getInfo() == this->finishVertex.getInfo());
}

template<class T>
void AStar<T>::removeVertexFromQueue() {
    this->pQueueWeighted.erase(this->topVertex);
}

template<class T>
void AStar<T>::updateVertexOnQueue(const AStarVertex<T> &curr, set<AStarVertex<T>> &queue) {
    for (auto & it : queue) {
        if (it.getInfo() == curr.getInfo()) {
            if (curr.getHeuristicWeight() < it.getHeuristicWeight()) {
                queue.erase(it);
                queue.insert(curr);
            }
            return;
        }
    }
}

template<class T>
void AStar<T>::buildPath() {

    this->solutionTotalCost = this->topVertex.getTotalWeight();
    this->checkedVertexes.insert(AStarVertex<T>(topVertex));

    lastSolution.clear();

    T currVertexId = this->finishVertex.getInfo();
    while (currVertexId != numeric_limits<T>::max()) {
        lastSolution.insert(lastSolution.begin(), currVertexId);
        currVertexId = checkedVertexes.find(currVertexId)->getLastInfo(); //pode estar aqui um problema
    }
}

#endif //CAL_ASTAR_H
