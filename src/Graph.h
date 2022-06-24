#pragma once

#include <vector>
#include <queue>
#include <climits>
#include <limits>
#include <iterator>
#include <iostream>
#include <unordered_set>
#include <cmath>
#include "GraphViewer/cpp/graphviewer.h"

using namespace std;

#define INFINITE_INT std::numeric_limits<double>::max()

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

//---------------------------------------Vertex
template <class T>
class Vertex {
protected:
	T info;
	void addEdge(Vertex<T> *dest, double w);
	int dist;
	vector<Edge<T>> adj;
	Vertex<T> * path;
	bool visited;
	int x;
	int y;
public:
	vector<Vertex<T>* > in;//the Vertexes that go into this vertex
	Vertex();
	Vertex(T info);
	friend class Graph<T>;
	bool hidden;//if this vertex is transversable
	size_t posAtVec;//saves the position of the vertex in the graph's vertexSet
	bool removeEdgeTo(Vertex<T> *d);
	void addEdgeTo(Vertex<T> *d, double w);
	T getInfo() const;
	void setVisited(bool v);
	bool getVisited();
	float getX() const;
	float getY() const;
	void setX(float x);
	void setY(float y);
	vector<Edge<T> > getAdj() const;
	Vertex<T> * getPath() const;
	double getDistanceToOtherVertex(Vertex<T> v) const;
	void clearAdj();
};

template <class T>
void Vertex<T>::clearAdj() {
	this->adj.clear();
}

template<class T>
struct VertexHash {
	 int operator()(const Vertex<T>&d) const {
		 int h1 = (int)d.getX();
		 int h2 = (int)d.getY();
		 return h1 ^ (h2 << 1);
	   }

    bool operator()(const Vertex<T> &d1,const Vertex<T> &d2) const {
        return d1.getInfo() == d2.getInfo();
    }


};

template <class T>
Vertex<T>::Vertex() {

}

template <class T>
using  VertexHashTable = std::unordered_set<Vertex<T>, VertexHash<T>, VertexHash<T>>;

template <class T>
vector<Edge<T> > Vertex<T>::getAdj() const {
	return adj;
}

template <class T>
Vertex<T> * Vertex<T>::getPath() const{
	return path;
}

template <class T>
float Vertex<T>::getX() const{
	return x;
}

template <class T>
float Vertex<T>::getY() const{
	return y;
}

template <class T>
void Vertex<T>::setVisited(bool v) {
	this->visited = v;
}

template <class T>
bool Vertex<T>::getVisited(){
	return visited;
}

template <class T>
void Vertex<T>::setX(float x) {
	this->x = x;
}

template <class T>
void Vertex<T>::setY(float y) {
	this->y = y;
}

template <class T>
double Vertex<T>::getDistanceToOtherVertex(Vertex<T> v) const {
	return sqrt((x - v.getX())*(x - v.getX()) + (y - v.getY())*(y - v.getY()));
}

template <class T>
Vertex<T>::Vertex(T info) : info(info), dist(0), path(nullptr), hidden(false){}

template <class T>
void Vertex<T>::addEdgeTo(Vertex<T> *d, double w) {
	adj.push_back(Edge<T>(d,w));
}

template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
	for (auto i = adj.begin(); i != adj.end(); i++){//iterate adjacent of current
		if (i->dest = d) {//if one of the adjacent is 
			for (auto vit = i->dest->in.begin(); vit != i->dest->in.end(); vit++) {//remove from the "in"
				if ((*vit) == this) {
					i->dest->in.erase(vit);
					adj.erase(i);//only delete it afterwards
					return true;
				}
			}
			return false;
		}
	}
	return false;
}

template<class T>
T Vertex<T>::getInfo() const {
	return info;
}

template <class T>
void Vertex<T>::addEdge(Vertex<T> *dest, double w) {
	Edge<T> edgeD(dest, w);//requires the template to have a distance method, this->info.distance(dest->info)
	//edgeD.dest->in++;// updates the number of vertexes going into the destination Vertex
	edgeD.dest->in.push_back(this);//Adds this vertex to the vector of vertexes going into the new one
	adj.push_back(edgeD);
}





//---------------------------------------Edge
template <class T>
class Edge {
	double weight;
	Vertex<T> * dest;
public:
	vector<Vertex<T> *> hidden;//vector of Vertexes that have been culminated into this edge in the preprocessing part
	Edge(Vertex<T> *d, double w);
	static Edge<T> mergeEdges(Edge<T> left, Edge<T> right, Vertex<T> * removedVertex);
	friend class Graph<T>;
	friend class Vertex<T>;
	double getWeight();
	Vertex<T> * getDest();
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w){}

template <class T>
double Edge<T>::getWeight() {
	return weight;
}

template <class T>
Vertex<T> * Edge<T>:: getDest() {
	return dest;
}


template<class T>
Edge<T> Edge<T>::mergeEdges(Edge<T> left, Edge<T> right, Vertex<T> * removedVertex) {
	Edge<T> res = Edge<T>(right.dest, left.weight + right.weight);
	//add the hidden vertexes by the correct order, from left to right
	for (auto v : left.hidden)
		res.hidden.push_back(v);
	res.hidden.push_back(removedVertex);
	for (auto v : right.hidden)
		res.hidden.push_back(v);
	return res;
}





//---------------------------------------Graph
template <class T>
class Graph {
	void dfs(Vertex<T> *v, vector<T> &res) const;
	vector<Vertex<T> *> vertexSet;
	static vector<vector<int>> distance;



public:
	void intitalizeDistance();
	std::vector<Vertex<T>> * getDistancefromVertex(Vertex<T> src);
	bool addVertex(const T &in);
	bool addVertexPointer(Vertex<T> * in);
	bool addEdge(Vertex<T> *vS, Vertex<T> *vD, double w, bool bothWays = false);
	bool addEdge(pair<Vertex<T> *, Vertex<T> *> sourceAndDest, double w, bool bothWays = false);
	bool removeVertex(const T &in);
	bool removeEdge(const T &sourc, const T &dest);
	vector<Vertex<T> * > getVertexSet() const;
	int getNumVertex() const;
	Vertex<T> * getVertex(const T &sourc) const;
	pair<Vertex<T> *, Vertex<T> *> getTwoVertexs(const T &sourc, const T &dest);
	static vector<T> getInfoFromVector(vector<Vertex<T> * > source);
	Vertex<T> * operator()(int n);
	void setVertex(vector<Vertex<T> *> vertexSet);

	Graph<T> * preProcessGraph();

	GraphViewer * displayOnGraphViewer(int width, int weight, bool dynamic, string vertexColor, string edgeColor);
	GraphViewer * showShortestPath(vector<T> path,int width, int weight, bool dynamic, string vertexColor, string edgeColor);

};

template <class T>
void Graph<T>::intitalizeDistance() {
	int vertexes = this->getNumVertex();
	vector<vector<int>> vec(vertexes, vector<int> (vertexes, INFINITE_INT));
	this->distance = vec;
}

template <class T>
std::vector<Vertex<T>> * Graph<T>:: getDistancefromVertex(Vertex<T> src) {
	return distance[src->info];
}


template<class T>
void Graph<T>::setVertex(vector<Vertex<T> *> vertexSet) {
	this->vertexSet = vertexSet;
}

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}
template <class T>
vector<Vertex<T> * > Graph<T>::getVertexSet() const {
	return vertexSet;
}

template <class T>
bool Graph<T>::addVertex(const T &in) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++)
		if ((*it)->info == in) return false;
	Vertex<T> *v1 = new Vertex<T>(in);
	v1->posAtVec = vertexSet.size();
	vertexSet.push_back(v1);
	return true;
}

template<class T>
inline bool Graph<T>::addVertexPointer(Vertex<T> * in) {
	typename vector<Vertex<T>*>::iterator it = vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite = vertexSet.end();
	for (; it != ite; it++)
		if ((*it)->info == in->info) return false;
	in->posAtVec = vertexSet.size();
	vertexSet.push_back(in);
	return true;
}

template <class T>
bool Graph<T>::removeVertex(const T &in) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++) {
		if ((*it)->info == in) {
			Vertex<T> * v= *it;
			vertexSet.erase(it);
			typename vector<Vertex<T>*>::iterator it1= vertexSet.begin();
			typename vector<Vertex<T>*>::iterator it1e= vertexSet.end();
			for (; it1!=it1e; it1++) {
				(*it1)->removeEdgeTo(v);
			}
			delete v;
			return true;
		}
	}
	return false;
}

template <class T>
bool Graph<T>::addEdge(Vertex<T> *vS, Vertex<T> *vD, double w, bool bothWays) {
	if (vS == nullptr || vD == nullptr) return false;
	vS->addEdge(vD,w);
	return true;
}

template<class T>
bool Graph<T>::addEdge(pair<Vertex<T>*, Vertex<T>*> sourceAndDest, double w, bool bothWays) {
	return addEdge(sourceAndDest.first, sourceAndDest.second, w, bothWays);
}

template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	int found=0;
	Vertex<T> *vS, *vD;
	while (found!=2 && it!=ite ) {
		if ( (*it)->info == sourc )
			{ vS=*it; found++;}
		if ( (*it)->info == dest )
			{ vD=*it; found++;}
		it ++;
	}
	if (found!=2) return false;
	return vS->removeEdgeTo(vD);
}



template<class T>
Vertex<T>* Graph<T>::getVertex(const T & source) const {
	for (auto v : vertexSet)
		if (v->info == source)
			return v;
	std::cout << "null T = " << source <<std::endl;
	return nullptr;
}


template<class T>
Graph<T> * Graph<T>::preProcessGraph() {
	cout << "Preprocessing...";
	Graph<T> * resultGraph = new Graph<T>;
	for (auto v : vertexSet)
		v->hidden = false;

	int countRedundant = 0;
	int countRedundantNotFixed = 0;
	for (auto v : vertexSet) {
		if (v->adj.size() == 1 && v->in.size() == 1) {
			
			countRedundantNotFixed++;
			Edge<T> enter = Edge<T>(nullptr, 0);
			bool found = false;
			typename vector<Edge<T>>::iterator eit;
			for (eit = v->in[0]->adj.begin(); eit != v->in[0]->adj.end(); eit++) {
				if (eit->dest == v) {
					
					found = true;
					enter = (*eit);
					break;
				}
			}
			if (found) {
				
				Edge<T> sum = Edge<T>::mergeEdges(enter, v->adj[0], v);
				v->in[0]->adj.erase(eit);
				v->in[0]->adj.push_back(sum);
				for (auto i = v->adj[0].dest->in.begin(); i != v->adj[0].dest->in.end(); i++) {
					if ((*i)->getInfo() == v->getInfo()) {
						v->adj[0].dest->in.erase(i);
						countRedundant++;
						break;
					}
				}

				v->adj[0].dest->in.push_back(v->in[0]);
				v->hidden = true;
			}
		}
	}
	for (auto v : vertexSet)
		if (!v->hidden)
			resultGraph->addVertexPointer(v);
	cout << "Done(" << countRedundant << " Redundant Nodes, "<< countRedundantNotFixed  <<" Found)" << endl;
	return resultGraph;
}

template<class T>
vector<T> Graph<T>::getInfoFromVector(vector<Vertex<T>*> source) {
	vector<T> result;
	for (auto v : source)
		result.push_back(v->getInfo());
	return result;
}


template<class T>
GraphViewer * Graph<T>::displayOnGraphViewer(int width, int weight, bool dynamic, string vertexColor, string edgeColor) {
	auto vertex=vertexSet.begin();
	double minX = (*vertex)->getX();
	double minY = (*vertex)->getY();
	double maxX = (*vertex)->getX();
	double maxY = (*vertex)->getY();

	for(auto & vertex : this->vertexSet){
	    if (vertex->getX() > maxX) {
	        maxX = vertex->getX();
	    }
	    else if (vertex->getX() < minX) {
	            minX = vertex->getX();
	        }

	        if (vertex->getY() > maxY) {
	            maxY = vertex->getY();
	        }
	        else if (vertex->getY() < minY) {
	            minY = vertex->getY();
	        }
	  }


	double graphHeight = maxY- minY;
	double graphWidth = maxX - minX;

	GraphViewer *gv = new GraphViewer(width, weight, dynamic);
	gv->createWindow(width, weight);
	gv->rearrange();
	gv->defineVertexColor(vertexColor);
	gv->defineEdgeColor(edgeColor);

	double yPercent, xPercent;
	int id = 0;
	for (auto vertex : vertexSet) {
		yPercent = 1.0 - ((vertex->getY() - minY-1)/graphHeight*0.9 + 0.05);
		xPercent = (vertex->getX() - minX-1)/graphWidth*0.9 + 0.05;
		gv->addNode(vertex->getInfo(), xPercent*600, yPercent*600);
		gv->setVertexLabel(vertex->getInfo(), to_string(vertex->getInfo()));
	}


	for (auto vertex : vertexSet) {
		for (auto edge : vertex->getAdj())
			gv->addEdge(id++, vertex->info, edge.dest->info, EdgeType::DIRECTED);
	}

	gv->defineEdgeCurved(false);
	gv->rearrange();
	return gv;
}

template<class T>
GraphViewer * Graph<T>::showShortestPath(vector<T> path,int width, int weight, bool dynamic, string vertexColor, string edgeColor) {
	if (path.size() <= 1) {
	        return nullptr;
	    }

	Vertex<T> *vertex = this->getVertex(path[0]);

	double minX = vertex->getX();
	double minY = vertex->getY();
	double maxX = vertex->getX();
	double maxY = vertex->getY();

	for(auto & vertex : this->vertexSet){
		if (vertex->getX() > maxX) {
			maxX = vertex->getX();
		}
		else if (vertex->getX() < minX) {
		    minX = vertex->getX();
		}

		if (vertex->getY() > maxY) {
		    maxY = vertex->getY();
		}
		else if (vertex->getY() < minY) {
		    minY = vertex->getY();
		}
	}


	double graphHeight = maxY- minY;
	double graphWidth = maxX - minX;

	GraphViewer *gv = new GraphViewer(width, weight, dynamic);
	gv->createWindow(width, weight);
	gv->rearrange();
	gv->defineVertexColor(vertexColor);
	gv->defineEdgeColor(edgeColor);

	double yPercent, xPercent;
		unsigned int id = 0;
		for (auto vertex : vertexSet) {
			yPercent = 1.0 - ((vertex->getY() - minY-1)/graphHeight*0.9 + 0.05);
			xPercent = (vertex->getX() - minX-1)/graphWidth*0.9 + 0.05;
			gv->addNode(vertex->getInfo(), xPercent*600, yPercent*600);
			gv->setVertexLabel(vertex->getInfo(), to_string(vertex->getInfo()));
		}


		for (auto vertex : vertexSet) {
			for (auto edge : vertex->getAdj()) {
				gv->addEdge(id++, vertex->info, edge.dest->info, EdgeType::DIRECTED);
			}
		}

		gv->defineEdgeCurved(false);

	for (size_t i = 1; i < path.size(); i++) {
		gv->addEdge(id, path.at(i - 1), path.at(i), EdgeType::DIRECTED);
	    gv->setEdgeDashed(id, false);
	    gv->setEdgeThickness(id, 5);
	    gv->setEdgeColor(id++, "orange");
	    if (i != path.size() - 1) {
	    	gv->setVertexColor(path.at(i), "yellow");
	    	gv->setVertexSize(path.at(i), 15);
	    }
	}

	    //Style start and finish
	gv->setVertexSize(path.at(0), 40);
	gv->setVertexSize(path.at(path.size() - 1), 40);
	gv->setVertexColor(path.at(0), "green");
	gv->setVertexColor(path.at(path.size() - 1), "red");
	gv->setVertexLabel(path.at(0), "Start location");
	gv->setVertexLabel(path.at(path.size() - 1), "Destination");
	gv->rearrange();

	return gv;
}


