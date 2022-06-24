//
// Created by Vítor Barbosa on 23/05/2019.
//

#ifndef CAL_PROJ_TWOOPT_H
#define CAL_PROJ_TWOOPT_H

#include "AStar.h"
#include "DFS.h"

template <class T>
class TwoOpt {
private:
    const Graph<T> &graph;
    DFS<T> dfs;        // To verify nodes accessibility within the graph
    AStar<T> astar;    // To calculate paths size within the graph


    // Performs a 2-opt swap in the visitOrder vector, using the 'i' and 'k' indexes as reference
    vector<T> twoOptSwap(vector<T> visitOrder, int i, int k) const;
    bool isVisitOrderValid(const vector<T> &visitOrder);
    double calcPathWeight(const vector<T> &visitOrder);
public:
    TwoOpt(const Graph<T> &graph);
    vector<T> twoOptImprovement(vector<T> visitOrder,double &visitOrderWeight);
    vector<T> buildSolution(vector<T> &visitOrder,double &solutionWeight);
};


template <class T>
TwoOpt<T>::TwoOpt(const Graph<T> & graph) : graph(graph), dfs(), astar(graph) {
	dfs.setGraph(graph);
}


template <class T>
vector<T> TwoOpt<T>::twoOptSwap(vector<T> visitOrder,int i,int k) const {
    while(i < k){
        visitOrder.at(i) ^= visitOrder.at(k);
        visitOrder.at(k) ^= visitOrder.at(i);
        visitOrder.at(i) ^= visitOrder.at(k);
        i++;
        k--;
    }

    return visitOrder;
}

template <class T>
bool TwoOpt<T>::isVisitOrderValid(const vector<T> &visitOrder) {
    for(int i = 0 ; i < visitOrder.size()-1 ; i++) {
        if(dfs.checkPath(visitOrder.at(i),visitOrder.at(i+1)))
            return false;
    }
    return true;
}

template <class T>
double TwoOpt<T>::calcPathWeight(const vector<T> &visitOrder) {
    double pathWeight = 0;
    for(int i = 0 ; i < visitOrder.size() -1;i++) {
        astar.calcOptimalPath(visitOrder.at(i),visitOrder.at(i+1));
        pathWeight += astar.getSolutionWeight();
    }
    return pathWeight;
}

template <class T>
vector<T> TwoOpt<T>::twoOptImprovement(vector<T> visitOrder,double &visitOrderWeight){
    double bestWeight = visitOrderWeight;
    vector<T> bestVisitOrder = visitOrder;

    T startNodeId = visitOrder.at(0);
    T finishNodeId = visitOrder.at(visitOrder.size()-1);

    unsigned int nodesSwapNo = visitOrder.size()-2;

    double lastBest = -1;

    while(lastBest != visitOrderWeight) {
        lastBest = visitOrderWeight;
        for(int i = 0;i <= nodesSwapNo-1;i++) {
            for(int k=i+1;k <= nodesSwapNo;k++){
                visitOrder = twoOptSwap(bestVisitOrder,i,k);

                if(!isVisitOrderValid(visitOrder)){
                    continue;
                }

                visitOrderWeight = calcPathWeight(visitOrder);

                if(visitOrderWeight < bestWeight) {
                    bestVisitOrder = visitOrder;
                    bestWeight = visitOrderWeight;
                }
            }
        }
    }
    return bestVisitOrder;
}


template <class T>
vector<T> TwoOpt<T>::buildSolution(vector<T> &visitOrder,double &solutionWeight) {
    solutionWeight = 0;
    vector<T> solution;

    for(int i = 0; i < visitOrder.size()-1;i++) {
        for(T i: astar.calcOptimalPath(visitOrder.at(i),visitOrder.at(i+1)))
            solution.push_back(i);

        solutionWeight += astar.getSolutionWeight();

        if(i!=visitOrder.size()-2){
            solution.pop_back();
        }
    }

    return solution;
}

#endif //CAL_PROJ_TWOOPT_H
