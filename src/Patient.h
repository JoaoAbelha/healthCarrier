#pragma once
#include <limits>
#include "Graph.h"

const int INF = std::numeric_limits<int>::max(); /*a mudar isto*/

class Patient {
private:
    char sickness;
    int time;
    int id;
    static int idNo;
public:
    Patient(char sickness = 'U',int  time = INF);
    int getId() const;
    char getSickness() const;
    int getTime() const;
    Patient & setSickness(char sickness);
    void atualizeTime(int time);
    bool inTime(int time) const;
    bool operator==(Patient &p) const;
    bool operator<(Patient &p) const;
};
