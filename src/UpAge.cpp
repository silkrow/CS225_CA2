#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <fstream>

#include "UpAge.h"
#include "MeTPerson.h"

using namespace std;

/*
Class UpAge is mainly a heap for updating ageGroup
*/

UpAge::UpAge(){//Initialization
    this->siz = 0;
}

void UpAge::change(int x, int y){//swap the x-th element and y-th element in the heap
    MeTPerson* tmp = this->dbP[x];
    this->dbP[x] = this->dbP[y];
    this->dbP[y] = tmp;
}

void UpAge::insert(MeTPerson* newp){//insert a new element into the heap
    this->siz++;
    this->dbP[this->siz] = newp;
    pushup(this->siz);
}

void UpAge::pushup(int p){//Adjust an element upward until reaching its proper position
    while(p > 1){
        if(cmp(dbP[p], dbP[p/2]) == 1){
            change(p, p/2);
            p /= 2;
        }else break;
    }
}

void UpAge::pop(){//delete the root of the heap
    dbP[1] = dbP[this->siz];
    this->siz--;
    pushdown(1);
} 

void UpAge::pushdown(int p){//Adjust an element downward until reaching its proper position
    int s = p * 2;
    int k = this->siz;
    while(s <= k){
        if(s < k && cmp(dbP[s],dbP[s+1]) == 0) s++; // s+1 is better than s
        if(cmp(dbP[s],dbP[p]) == 1){ // s is better than p
            change(s, p);
            p = s,s = p * 2;
        }
        else break;
    }
}

MeTPerson* UpAge::top(){//return the value of heap root
    return this->dbP[1];
}

bool UpAge::cmp(MeTPerson* p1, MeTPerson* p2){
    // if the date for p1 getting into the next ageGroup is earlier than p2, return 1
    // else return 0
    string s1 = p1->nxtUpdate;
    string s2 = p2->nxtUpdate;
    int by1,bm1,bd1,by2,bm2,bd2;
    by1=(s1[0]-'0')*1000+(s1[1]-'0')*100+(s1[2]-'0')*10+(s1[3]-'0');
    bm1=(s1[5]-'0')*10+(s1[6]-'0');
    bd1=(s1[8]-'0')*10+(s1[9]-'0');
    by2=(s2[0]-'0')*1000+(s2[1]-'0')*100+(s2[2]-'0')*10+(s2[3]-'0');
    bm2=(s2[5]-'0')*10+(s2[6]-'0');
    bd2=(s2[8]-'0')*10+(s2[9]-'0');
    if (by1<by2) return 1;
    if (by1==by2 && (bm1<bm2)) return 1;
    if (by1==by2 && bm1==bm2 && bd1<bd2)  return 1;
    return 0; 
}
