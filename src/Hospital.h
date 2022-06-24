#pragma once

#include "Graph.h"
#include <vector>

template<class T>
class Hospital : public Vertex<T> {

    private:
    char typeOfTreatments;
    bool considerTreatment;

    public:
    Hospital();
    Hospital(Vertex<T> vertex);
    Hospital(unsigned int id, double x, double y, std::string name);
    Hospital(unsigned int id, double x, double y, std::string name, char type);
    Vertex<T> getVertex();
    void setTreatment(const char type);
    char getTreatment() const;
};

template<class T>
Hospital<T>::Hospital() : Vertex<T>() , considerTreatment(false) {

}

template<class T>
Hospital<T>::Hospital(Vertex<T> vertex) : Vertex<T>(vertex.getInfo())  {

}

template<class T>
Hospital<T>::Hospital(unsigned int id, double x, double y, std::string name) : Vertex<T>(id,x,y,name) {
}


    
template<class T>
Vertex<T> Hospital<T>::getVertex() {
    return Vertex<T>(this->info);
}



template <class T>
void Hospital<T>::setTreatment(const char type) {
	this->typeOfTreatments = type;
}

template <class T>
char Hospital<T>::getTreatment() const {
	return this->typeofTreatments;
}

