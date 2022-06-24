#include "Patient.h"

int Patient::idNo = 1;

 Patient::Patient(char sickness,int time) :sickness(sickness), time(time) {
	 id = idNo++;
 }

 int Patient::getId() const {
	 return this->id;
 }

char Patient::getSickness() const {
    return this->sickness;

}

int Patient::getTime() const {
    return this->time;

}

Patient & Patient::setSickness(char sickness) {
    this->sickness = sickness;
    return *this;
}

 void Patient::atualizeTime(int time) {
     this->time-=time;
 }

 bool Patient::inTime(int time) const {
     return (this->time - time ) >= 0;
 }

 bool Patient::operator==(Patient &p) const {
	 return p.getSickness() == this->sickness && p.getTime() == this->time;
 }

 bool Patient::operator<(Patient &p) const {
	 return this->time < p.getTime();
 }
