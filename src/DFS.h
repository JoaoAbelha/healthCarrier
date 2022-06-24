#pragma once

#include "Graph.h"
#include <stack>
#include <vector>

template < class T>
class DFS {
    private:
     Graph<T> mapa;
     std::stack<Vertex<T>> vertexStack; /*auxiliar*/
     std::vector<Vertex<T>> dfsOrder;
     VertexHashTable<T> vertexVisited;
     bool isVertexVisited(const Vertex<T>  & vertex ) const;


     public:
     DFS();
     DFS(Graph<T> map);
     std::vector<Vertex<T>> algorithm(Vertex<T> src);
     std::vector<Vertex<T>> algorithmComplete(Vertex<T> src);
     bool k();
     void setGraph(Graph<T> map);
     VertexHashTable<T> getVertexVisited();
     bool checkPath(T startNodeId, T finishNodeId);
};


template <class T>
DFS<T>::DFS()  {

}

template <class T>
DFS<T>::DFS(Graph<T> map)  {
	setGraph(map);
}


template <class T>
void DFS<T>::setGraph(Graph<T> map) {
	for (Vertex<T> * v : map.getVertexSet())
        for (auto edge : v->getAdj())
        	if (!isVertexVisited(*edge.getDest()))
        		vertexStack.push(*edge.getDest());

	this->mapa = map;
	vertexVisited.clear();
	dfsOrder.clear();
}


/*verifica se um vertice ja foi visitado*/
template <class T>
bool DFS<T>::isVertexVisited(const  Vertex<T> & vertex ) const {
	if (vertexVisited.find(vertex) == vertexVisited.end())
		return false;
	return true;

}

/*iterative dfs para vertices alcancaveis*/
template <class T>
std::vector<Vertex<T>> DFS<T>::algorithm(Vertex<T> src) {
	vertexVisited.clear();
    vertexStack.push(src);
    while(!vertexStack.empty()) {
        Vertex<T> top = vertexStack.top();
        vertexStack.pop();

        if(!isVertexVisited(top)) {
            vertexVisited.insert(top);
            dfsOrder.push_back(top);
        }

        for (auto edge : top.getAdj()) {
        	if (!isVertexVisited(*edge.getDest())){
        		vertexStack.push(*edge.getDest());
        	}
        }

    }

    return dfsOrder;
}

/*retorna a dfs para todos os vertices do grafo*/
template <class T>
std::vector<Vertex<T>> DFS<T>::algorithmComplete(Vertex<T> src) {
    vertexVisited.clear(); /*meter tudo como nao visitado*/

    for(unsigned int i = 0 ; i < mapa.getNumVertex(); i++) {
        Vertex<T> atual = mapa.getNumVertex()[i];
        if (!isVertexVisited(atual)) {
            algorithm(atual);
        }
    }

    return dfsOrder;
}

template <class T>
bool DFS<T>::k() {

	Vertex<T> atual = *mapa.getVertexSet()[0];
	algorithm(atual);

	if (this->dfsOrder.size() != mapa.getNumVertex())
		return false;

	return true;
}

template <class T>
VertexHashTable<T> DFS<T>::getVertexVisited() {
	return vertexVisited;
}

template <class T>
bool DFS<T>::checkPath(T startNodeId, T finishNodeId) {
    Vertex<T> startVertex;
    Vertex<T> finishVertex;

    startVertex = *mapa.getVertex(startNodeId);
    finishVertex = *mapa.getVertex(finishNodeId);


    vertexVisited.clear();
    algorithm(startVertex);

    if (vertexVisited.find(finishVertex) == vertexVisited.end())
    	return false;

    return true;
}

