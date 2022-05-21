#ifndef REJ_POS
#define REJ_POS

#include<queue>
#include "MeTPerson.h"
#include "FibHeap.h"
#include "BPlusTree.h"
#include "BTree.h"
#include<vector>


using namespace std;

class RegPos{
    public:
    RegPos(int id);
    int RegID;
    vector<int> locList;
    //int locList[500005];
    //queue<int> RegPid;
    int cur; // current update pos
    int cnt; // total number of person
    void forwardInf(BPTree* B1,BTree* B2, MeTPerson** db);
};

#endif
