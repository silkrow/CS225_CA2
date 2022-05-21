#ifndef INT_NODE
#define INT_NODE

//#include "MeTPerson.h"
#include<vector>

using namespace std;

class MeTPerson;

class IntNode{
    public:
    IntNode();
    //int maincap; // capacity for main block
    //int ofcap; // capacity for overflow block
    int maincnt; // real size of main block, <=maincap
    int ofcnt; // real size of overflow block, <=ofcap
    int tombcnt;

    const int maincap = 10; // capacity for main block
    const int ofcap = 3; // capacity for overflow block
    const int mergefactor=8;    //If less than 1/n in a block, merge with the right one 

    IntNode* leftNeighbor;
    IntNode* rightNeighbor;
    void Adjustof();
    int Num();
    void insertB(MeTPerson* tmp);
    int Find(MeTPerson* tmp);   //-2: found in of  -1:not found  otherwise:found in main, return index
    void deleteB(MeTPerson* tmp);
    MeTPerson* topPer();
    
    MeTPerson* mainB[13];
    MeTPerson* ofB[5];
    void mergeB();
    void splitB();
    void sortB();
};

#endif
