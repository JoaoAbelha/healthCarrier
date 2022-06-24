#pragma once
#include "Graph.h"
#include "Ambulance.h"
#include "HealthCenter.h"
#include "Hospital.h"
#include "TSPNearestNeighbor.h"
#include "TwoOpt.h"
#include <algorithm>
#include <cmath>
#include <unordered_map>

template <class T>
class CompleteGraph {
public:
	Graph<int> * graphOriginal;

	Graph<int> * graph;

	std::vector<Edge<T> *> roads;

	std::vector<Ambulance<T>*> ambulances;

	unordered_map<T, HealthCenter<T>*> healthCenters;

	std::vector<Hospital<T> *> hospitals;

	CompleteGraph();


	CompleteGraph<T> & preProcessGraph();

	bool addAmbulance(Ambulance<T> * a);

	bool addHospital(Hospital<T> * h);

	bool addHealthCenter(HealthCenter<T> * c);

	vector<HealthCenter<T> *> getHealthCenter();

	vector<T> generateOneAmbulanceMultipleHealthCenter(Ambulance<T> * a, Hospital<T> * h);

	int pickUpPatients(HealthCenter<T> *hc, Ambulance<T> *a,int number);

	int pickUpPatientsWithRestrictions(HealthCenter<T> *hc, Ambulance<T> *a);

	vector<vector<T>> generateMultipleAmbulancesMultipleHealthCenter(Hospital<T> *h, Vertex<T> * start,vector<T> pois);

	vector<vector<T>> createType(Hospital<T> * h, char type, Vertex<T> * start);

	vector<vector<T>> generateMultipleAmbulancesMultipleHealthCenterWithRestrictions(vector<Hospital<T>*> destinos, Vertex<T> * start);
};

template<class T>
CompleteGraph<T>::CompleteGraph() {
}

template<class T>
CompleteGraph<T> & CompleteGraph<T>::preProcessGraph() {
	graph = graphOriginal->preProcessGraph();
	return *this;
}

template<class T>
bool CompleteGraph<T>::addAmbulance(Ambulance<T> * a) {
	if (find(ambulances.begin(), ambulances.end(), a) == ambulances.end()) { //not in the vector, so it will be added
		ambulances.push_back(a);
		return true;
	}
	return false;
}

template<class T>
bool CompleteGraph<T>::addHospital(Hospital<T> * h) {
	if (find(hospitals.begin(), hospitals.end(), h) == hospitals.end()) { //not in the vector, so it will be added
		hospitals.push_back(h);
		return true;
	}
	return false;
}

template<class T>
bool CompleteGraph<T>::addHealthCenter(HealthCenter<T> * c) {
	if (healthCenters.find(c->getInfo()) == healthCenters.end()) { //not in the vector, so it will be added
		healthCenters.insert(pair<int, HealthCenter<T>*>(c->getInfo(), c));
		return true;
	}
	return false;
}

template<class T>
std::vector<HealthCenter<T> *> CompleteGraph<T>::getHealthCenter() {
	std::vector<HealthCenter<T> *> res;
	for (auto c : healthCenters)
		res.push_back(c.second);
	return res;
}

template<class T>
vector<T> CompleteGraph<T>::generateOneAmbulanceMultipleHealthCenter(
		Ambulance<T> * a, Hospital<T> * h) {
	vector<T> result, temp;


	Vertex<T> src = *(a->getExitNode());

	Vertex<T> dest = h->getVertex();

	TSPNearestNeighbor<T> nearestNeighbor(*graph);
	TwoOpt<T> twoOpt(*graph);

	vector<T> healthCentersIds = a->getPickUp();

	result = nearestNeighbor.calcPath(src.getInfo(), dest.getInfo(),
			healthCentersIds);

	double weight;
	double initialWeight = nearestNeighbor.getSolutionWeight();

	result = twoOpt.twoOptImprovement(nearestNeighbor.getVisitOrder(),initialWeight);
	result = twoOpt.buildSolution(result, weight);

	if (weight - nearestNeighbor.getSolutionWeight() == 0) {
		cout << "could not obtain a better solution with 2-Opt" << endl;
	} else {
		cout << "2-Opt improved the solution to a distance of " << weight << "m."
				<< endl;
		cout << "Total improvement:  "
				<< nearestNeighbor.getSolutionWeight() - weight << "m." << endl;
	}

	return result;
}


template <class T>
int CompleteGraph<T>::pickUpPatients(HealthCenter<T> *hc, Ambulance<T> *a,int number) {
	for(auto &patient:hc->getPatients()) {
		if(!number)
			break;
		a->addPatient(patient);
		hc->removePatient(patient);
		number--;
	}
}


template<class T>
vector<vector<T>> CompleteGraph<T>::generateMultipleAmbulancesMultipleHealthCenter(Hospital<T> *h, Vertex<T> * start,vector<T> healthCentersIds) {
	srand(time(NULL));

	vector<vector<T>> result;

	Vertex<T> dest = h->getVertex();

	TSPNearestNeighbor<T> nearestNeighbor(*graph);

	nearestNeighbor.calcPath(start->getInfo(), dest.getInfo(), healthCentersIds);
	vector<T> bestPath = nearestNeighbor.getVisitOrder();

	bestPath.erase(bestPath.begin());
	bestPath.erase(bestPath.end()-1);

	this->ambulances.push_back(new Ambulance<T>(start,rand() % 10 + 1));


	for(int i = 0;i<bestPath.size();){
		HealthCenter<T> *healthC = this->healthCenters.find(bestPath[i])->second;
		if(healthC->getPatients().size() == ambulances[ambulances.size()-1]->getCapacity()-ambulances[ambulances.size()-1]->getPatients().size()){
			this->pickUpPatients(healthC,ambulances[ambulances.size()-1],healthC->getPatients().size());
			ambulances[ambulances.size()-1]->addPickUp(bestPath[i]);
			this->ambulances.push_back(new Ambulance<T>(start,rand() % 10 + 1));
			i++;
		}
		else if(healthC->getPatients().size() > ambulances[ambulances.size()-1]->getCapacity()-ambulances[ambulances.size()-1]->getPatients().size()) {
			this->pickUpPatients(healthC,ambulances[ambulances.size()-1],ambulances[ambulances.size()-1]->getCapacity()-ambulances[ambulances.size()-1]->getPatients().size());
			ambulances[ambulances.size()-1]->addPickUp(bestPath[i]);
			this->ambulances.push_back(new Ambulance<T>(start,rand() % 10 + 1));
		}
		else {
			this->pickUpPatients(healthC,ambulances[ambulances.size()-1],healthC->getPatients().size());
			ambulances[ambulances.size()-1]->addPickUp(bestPath[i]);
			i++;
		}
	}
	for(int i = 0;i < ambulances.size() ; i++) {
		result.push_back(generateOneAmbulanceMultipleHealthCenter(ambulances[i] , h));
	}

	int counter=0;
	for(int i = 0; i < ambulances.size(); i++) {
		counter +=  ambulances[i]->getPatients().size();
		std::cout << "Treated patients by ambulance = " << ambulances[i]->getPatients().size() << std::endl;
	}

	std::cout << "Total Treated patients = " << counter << std::endl;
	return result;
}

 /////////////////////////////////// METER UM TEMPO ALEATORIO
Patient * getRandomPatient(char type, int time) {
	int waitTime = rand()%3*time+time;
	return new Patient(type,waitTime);
}

bool checkPois(vector<int> ambulancePois,vector<Patient*> patients) {
	if(ambulancePois.size() < 2)
		return true;
}

vector<int> createRandom(int min, int max) {
	vector<int> result;
	int number = rand() % max;
	for(int i=0; i < number; i++) {
		result.push_back(rand() % max + min);
	}
	return result;

}

template <class T>
vector<vector<T>> CompleteGraph<T>::createType(Hospital<T> * h,char type,  Vertex<T> * start) {
	h->setTreatment(type);

	TSPNearestNeighbor<T> nearestNeighbor(*graph);
	AStar<T> astar(*graph);

	vector<vector<T>> result;
	vector<T> healthCentersIds = createRandom(0,healthCentersIds.size());
	Vertex<T> dest = h->getVertex();

	vector<T> bestPath = nearestNeighbor.calcPath(start->getInfo(),
	dest.getInfo(), healthCentersIds);
	bestPath.erase(bestPath.begin());
	bestPath.erase(bestPath.end() - 1);

	vector<HealthCenter<T> *> centros; /* subset de centros a visitar*/

	for(int i = 0; i < healthCentersIds.size(); i++) {
		centros.push_back(this->healthCenters[healthCentersIds[i]]);
	}

	for(int i = 0; i < centros.size(); i++){
		double dist = 0;
		astar.calcOptimalPath(healthCentersIds[i],dest.getInfo());
		dist += astar.getSolutionWeight();
		centros[i]->setDistanceToHospital(ceil(dist));
	}

		// pacientes distribuidos pelos centros
	int patientscounter = 0;
	for (auto & healthCenter : centros) {
		healthCenter.second.clear(); //apagar os pacientes porque foram tratados no ultimo create type
		int patientsNo = rand() % 10 + 1;
		for (int i = 0; i < patientsNo; i++) {
			Patient * patient= getRandomPatient(type, healthCenter->getDistance());
			healthCenter.second->addPatient(patient);
			patientscounter++;
		}
	}


	for(int i = 0; i < centros.size()-1; i++){
		double dist = 0;
		astar.calcOptimalPath(healthCentersIds[i],healthCentersIds[i+1]);
		dist += astar.getSolutionWeight();
		centros[i]->setDistance(ceil(dist)+centros[i+1]->getDistance());
	}

	vector<T> pois;
	for(int i = 0 ; i<centros.size(); i++) {
		pois.push_back(centros[i]->getInfo());
		if((*centros[i]->getPatients().begin())->getTime() < centros[i]->getDistance()){
			vector<vector<T>> result2 = this->generateMultipleAmbulancesMultipleHealthCenter(h,start,pois);
			result.insert(result.end(),result2.begin(),result2.end());
			pois.clear();
		}
	}

	if(!pois.empty()) {
		vector<vector<T>> result2 = this->generateMultipleAmbulancesMultipleHealthCenter(h,start,pois);
		result.insert(result.end(),result2.begin(),result2.end());
	}
}

template<class T>
vector<vector<T>> CompleteGraph<T>::generateMultipleAmbulancesMultipleHealthCenterWithRestrictions(vector<Hospital<T>*> destinos, Vertex<T> * start) {
	srand(time(NULL));

	vector<vector<T>> result;

	int i = 97;

	for (auto & destino : destinos) {
		vector<vector<T>> r = createType(destino,(char)i);
		result.insert(result.end(),r.begin(),r.end());
		i++;
	}

	return result;
}

