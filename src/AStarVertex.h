#pragma once

#include <set>
#include <float.h>
#include "Graph.h"
#include <unordered_set>

template<class T>
class AStarVertex : public Vertex<T> {
    T lastInfo;
    double totalWeight;
    double distToFinish;
    double heuristicWeight;
public:
    AStarVertex();
    AStarVertex(T info);
    AStarVertex(T info, T lastInfo);
    AStarVertex(const Vertex<T> &vertex);
    AStarVertex(double distToFinish);
    AStarVertex(const Vertex<T> &vertex, T lastInfo , double totalWeight, double distToFinish);
    AStarVertex(const Vertex<T> &vertex, double totalWeight, double distToFinish);
    double getTotalWeight() const;
    double getHeuristicWeight() const;
    T getLastInfo() const;
    bool operator<(const AStarVertex &v) const;
    bool operator==(const AStarVertex<T>& v) const;
    bool operator!=(const AStarVertex<T>& v) const;
};

template<class T>
struct AStarVertexHash {
    bool operator()(const AStarVertex<T> &v1, const AStarVertex<T> &v2) const {
        return v1.getInfo() == v2.getInfo();
    }

    int operator()(const AStarVertex<T> &v) const {
        return v.getInfo();
    }
};

template <class T>
using  AStarVertexHashTable = std::unordered_set<AStarVertex<T>, AStarVertexHash<T>, AStarVertexHash<T>>;

template<class T>
AStarVertex<T>::AStarVertex() {
    this->lastInfo = numeric_limits<T>::max();
    this->totalWeight = DBL_MAX;
    this->distToFinish = DBL_MAX;
    this->heuristicWeight = DBL_MAX;
}

template<class T>
AStarVertex<T>::AStarVertex(T info) {
    this->info = info;
    this->lastInfo = numeric_limits<T>::max();
    this->totalWeight = DBL_MAX;
    this->distToFinish = DBL_MAX;
    this->heuristicWeight = DBL_MAX;
}

template<class T>
AStarVertex<T>::AStarVertex(T info, T lastInfo) {
    this->info = info;
    this->lastInfo = lastInfo;
    this->totalWeight = DBL_MAX;
    this->distToFinish = DBL_MAX;
    this->heuristicWeight = DBL_MAX;
}

template<class T>
AStarVertex<T>::AStarVertex(const Vertex<T> &vertex) {
	this->adj = vertex.getAdj();
    this->info = vertex.getInfo();
    this->lastInfo = numeric_limits<T>::max();
    this->totalWeight = DBL_MAX;
    this->distToFinish = DBL_MAX;
    this->heuristicWeight = DBL_MAX;
    this->x = vertex.getX();
    this->y = vertex.getY();
}

template<class T>
AStarVertex<T>::AStarVertex(const Vertex<T> &vertex, T lastInfo , double totalWeight, double distToFinish){
	this->adj = vertex.getAdj();
	this->info = vertex.getInfo();
    this->lastInfo = lastInfo;
    this->totalWeight = totalWeight;
    this->distToFinish = distToFinish;
    this->heuristicWeight = this->distToFinish + this->totalWeight;
    this->x = vertex.getX();
    this->y = vertex.getY();
}

template<class T>
AStarVertex<T>::AStarVertex(const Vertex<T> &vertex, double totalWeight, double distToFinish){
	this->adj = vertex.getAdj();
	this->info = vertex.getInfo();
	this->lastInfo = numeric_limits<T>::max();
    this->totalWeight = totalWeight;
    this->distToFinish = distToFinish;
    this->heuristicWeight = this->distToFinish + this->totalWeight;
    this->x = vertex.getX();
    this->y = vertex.getY();
}

template<class T>
double AStarVertex<T>::getTotalWeight() const {
    return totalWeight;
}

template<class T>
double AStarVertex<T>::getHeuristicWeight() const {
    return heuristicWeight;
}

template<class T>
T AStarVertex<T>::getLastInfo() const {
    return lastInfo;
}

template<class T>
bool AStarVertex<T>::operator<(const AStarVertex &v) const {
    if(this->heuristicWeight == v.getHeuristicWeight())
        return this->info < v.getInfo();
    return this->heuristicWeight < v.getHeuristicWeight();
}

template<class T>
bool AStarVertex<T>::operator==(const AStarVertex<T>&v) const{
    return this->info == v.getInfo();
}

template<class T>
bool AStarVertex<T>::operator!=(const AStarVertex<T>&v) const{
    return !(this->info == v.getInfo());
}
