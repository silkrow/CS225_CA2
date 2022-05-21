#ifndef PER_DB
#define PER_DB

//#include "MeTPerson.h"
//#include "IntNode.h"

//using namespace std;

class IntNode;
class MeTPerson;

class PerDB{
    public:
    PerDB();
    IntNode* topNode;
    IntNode* createBlock(); // initialize
    MeTPerson* popTopn(int num); // get top num elements in the database
    
    void insertPer(MeTPerson* tmp, IntNode* block);
    void deletePer(MeTPerson* tmp, IntNode* block);
    
    private:
};

#endif
