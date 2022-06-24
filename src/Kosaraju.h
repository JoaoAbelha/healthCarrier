#ifndef SRC_KOSARAJU_H_
#define SRC_KOSARAJU_H_
#include "DFS.h"
#include "TransposeGraph.h"

template<class T>
class Kosaraju {
	private:
	Graph<T> map;
	DFS<T> dfs;
	TransposeGraph<T> transpose;


	public:
	Kosaraju(Graph<T>map);
	bool algorithm();
};


template <class T>
Kosaraju<T>::Kosaraju(Graph<T> map) : dfs(), transpose(map) {
	this->map = map;

}

template <class T>
bool Kosaraju<T>::algorithm() {
	dfs.setGraph(map);
	if (!dfs.k())
		return false;
	dfs.setGraph(transpose.getTransposedGraph());
	return dfs.k();
}




#endif /* SRC_KOSARAJU_H_ */
