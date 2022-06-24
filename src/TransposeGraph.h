#ifndef SRC_TRANSPOSEGRAPH_H_
#define SRC_TRANSPOSEGRAPH_H_
#include "Graph.h"

template<class T>
class TransposeGraph {
private:
	const Graph<T> map;      	/*input*/
	Graph<T> transposedGraph;   /*output*/
	void algorithm();
public:
	TransposeGraph() {};
	TransposeGraph(const Graph<T> & map);
	Graph<T>getTransposedGraph();
};

template <class T>
TransposeGraph<T>::TransposeGraph(const Graph<T> & map) : map(map) {
	this->transposedGraph.setVertex(map.getVertexSet());  /*os vertices sao os mesmos*/
}

template<class T>
void TransposeGraph<T>::algorithm() { /*arestas devem ser invertidas* - ver se da para usar posAtVec*/
	for(unsigned int i = 0; i < this->map.getNumVertex(); i++)
		for(unsigned int j = 0 ; this->map.getVertexSet()[i].getAdj(); j++) {
			this->transposedGraph.addEdge(map.getVertexSet()[i].getAdj()[j],map.getVertexSet()[i], map.getVertexSet()[i].getAdj()[j].getWeight());
		}
}

template<class T>
Graph<T> TransposeGraph<T>::getTransposedGraph() {
	return this->transposedGraph;
}


#endif /* SRC_TRANSPOSEGRAPH_H_ */
