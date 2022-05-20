
#ifndef PER_DB
#define PER_DB

#include "MeTPerson.h"
#include "IntNode.h"

using namespace std;

class PerDB{
    public:
    PerDB();
    
    IntNode* topNode;
    void createBlock(); // initialize
    MeTPerson* popTopn(int num); // get top num elements in the database
    
    void insertPer(MeTPerson* tmp, IntNode* block);
    void deletePer(MeTPerson* tmp, IntNode* block);
    
    private:
};

#endif
