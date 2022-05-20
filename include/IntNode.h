#ifndef INT_NODE
#define INT_NODE

#include "MeTPerson.h"
#include<vector>

using namespace std;

class IntNode{
    public:
    IntNode();
    //int maincap; // capacity for main block
    //int ofcap; // capacity for overflow block
    int maincnt; // real size of main block, <=maincap
    int ofcnt; // real size of overflow block, <=ofcap
    IntNode* leftNeighbor;
    IntNode* rightNeighbor;
    void insertB(MeTPerson* tmp);
    void deleteB(MeTPerson* tmp);
    MeTPerson* topPer();
    
    private:
    vector<MeTPerson*> mainB;
    vector<MeTPerson*> ofB;
    void mergeB();
    void splitB();
    void sortB();
};

#endif