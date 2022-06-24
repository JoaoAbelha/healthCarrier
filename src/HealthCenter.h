#pragma once

#include <map>
#include "Graph.h"
#include "Patient.h"
#include "Hospital.h"

template<class T>
class HealthCenter : public Vertex<T> {
    private:
    std::map<char,set<Patient*>> patients;
    int distance;

    public:
    HealthCenter(T info);
    void addPatient(Patient *Patient);
    void removePatient(Patient *Patient);
    std::set<Patient*> getPatients(char type = 'U');
    void setDistance(int dist);
    int getDistance() const;
};

template <class T>
HealthCenter<T>::HealthCenter(T info) : Vertex<T>(info){}



template<class T>
void HealthCenter<T>::addPatient(Patient * patient) {
	patients[patient->getSickness()].insert(patient);
}

template<class T>
void HealthCenter<T>::removePatient(Patient *patient) {
	for(auto it = this->patients[patient->getSickness()].begin() ; it != this->patients[patient->getSickness()].end() ; it++) {
		if((*it)->getId() == patient->getId()) {
			this->patients[patient->getSickness()].erase(it);
		}
	}
}

template<class T>
std::set<Patient*> HealthCenter<T>::getPatients(char type){
	return this->patients[type];
}

template<class T>
void HealthCenter<T>::setDistance(int dist) {
	this->distance = dist;
}

template <class T>
int HealthCenter<T>::getDistance() const{
	return distance;
}
