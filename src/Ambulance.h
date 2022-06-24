#pragma once
#include "Graph.h"
#include "Patient.h"

template<class T>
class Ambulance {
    
    private:
    Vertex<T> *startingNode;
    unsigned int capacity;
    char sickness;
    vector<Patient*> patients;
    vector<T> pickups;

    public:
    Ambulance();
    Ambulance(Vertex<T> *StartingNode, unsigned int capacity, char type = 'U'); /*U- nao importa para iter 1 e 2*/
    Vertex<T>* getExitNode() const;
    Ambulance & setExitNode(Vertex<T> *ExitNode);
    unsigned int getCapacity() const;
    vector<T> getPickUp() const;
    Ambulance &setCapacity(unsigned int capacity);
    void setPickUp(vector<T> pickups);
    Ambulance & addPatient(Patient *patient);
    void addPickUp(T info);
    char getSickness() const;
    Ambulance &setSickness(char sickness);
    bool isFull();
    bool operator<(const Ambulance<T> &a);
    vector<Patient*> getPatients() { return this->patients; }
};

template<class T>
Ambulance<T>::Ambulance() {
}

template<class T>
Ambulance<T>::Ambulance(Vertex<T>* StartingNode, unsigned int capacity, char type)
: startingNode(StartingNode), capacity(capacity), sickness(type) {}

template<class T>
Vertex<T>* Ambulance<T>::getExitNode() const {
    return this->startingNode;
}

template<class T>
Ambulance<T> & Ambulance<T>::setExitNode(Vertex<T>* ExitNode) {
     this->startingNode = ExitNode;
     return *this;
 }

template<class T>
unsigned int Ambulance<T>::getCapacity() const {
    return this->capacity;
}

template<class T>
vector<T> Ambulance<T>::getPickUp() const {
	return this->pickups;
}

template<class T>
Ambulance<T> & Ambulance<T>::setCapacity(unsigned int capacity) {
     this->capacity = capacity;
     return *this;
 }

template <class T>
void Ambulance<T>::setPickUp(vector<T> pickup) {
	this->pickups=pickup;
}

template<class T>
Ambulance<T> & Ambulance<T>::addPatient(Patient* patient) {
    this->patients.push_back(patient);
    return *this;
}

template <class T>
void Ambulance<T>::addPickUp(T info) {
	this->pickups.push_back(info);
}

template<class T>
char Ambulance<T>::getSickness() const {
    return this->sickness;
}

template<class T>
Ambulance<T> & Ambulance<T>::setSickness(char sickness) {
    this->sickness = sickness;
    return *this;
}

template<class T>
bool Ambulance<T>::isFull() {
    return this->patients.size() == this->capacity;
}

template<class T>
bool Ambulance<T>::operator<(const Ambulance<T> &a) {
	return this->capacity<a.getCapacity();
}
