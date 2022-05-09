#ifndef NOD_PER
#define NOD_PER

using namespace std;
//#include "MeTPerson.h"
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
