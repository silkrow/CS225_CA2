#include "InjPos.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

using namespace std;

InjPos::InjPos(int id, int cap, double lo, double la){
    this->InjID = id;
    this->capacity = cap;
    this->longitude = lo;
    this->latitude = la;
}

void InjPos::updateCap(int newCap){
    this->capacity = newCap;
}
