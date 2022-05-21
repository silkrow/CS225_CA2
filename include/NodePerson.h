#ifndef NOD_PER
#define NOD_PER


//#include "MeTPerson.h"

//using namespace std;

class MeTPerson;

class NodePerson{
    public:
    MeTPerson* keyPerson;
    int degree;
    bool marked;
    NodePerson* left;
    NodePerson* right;
    NodePerson* parent;
    NodePerson* child;
};


#endif
