#ifndef PER_DB
#define PER_DB

#include "MeTPerson.h"
#include "IntNode.h"

using namespace std;

class PerDB{
    public:
    PerDB();

    int cntBlocks; // total number of blocks
    const int maincap = 10; // capacity for main block
    const int ofcap = 3; // capacity for overflow block
    
    IntNode* topNode;
    void createBlock(); // initialize
    MeTPerson* popTopn(int num); // get top num elements in the database
    
    void insertPer(MeTPerson* tmp, IntNode* block);
    void deletePer(MeTPerson* tmp, IntNode* block);
    
    private:
};

#endif