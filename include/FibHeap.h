#ifndef FIB_HEA
#define FIB_HEA

#include "NodePerson.h"
//#include "MeTPerson.h"


using namespace std;
class MeTPerson;

class FibHeap{
    public:
    int totNum;
    int maxDegree;
    NodePerson* mNode;
    FibHeap();  
    ~FibHeap(); 

    //insert a new node with keyp as newp into the Fibonacci heap
    NodePerson* insert(MeTPerson* newp);
    //combine a new heap in current FibHeap
    void combine(FibHeap* x);
    //remove the min node in fib heap, remove its PID
    int removeM();
    //update the keyp of x to newp
    void updKey(NodePerson* x, MeTPerson* newp);
    //delete a node
    void delNode(NodePerson* x);
    //destroy the fib heap
    void destroy();
    //print
    void printFib();

    private:

    //insert operation
    void addNode(NodePerson* node, NodePerson* root);//and the node before the root node in the double linked list
    void insertN(NodePerson* newn);//Insert newn in the FibHeap
	NodePerson* createN (MeTPerson* newp);//create a new node

    //merge operation
    void catList(NodePerson* a, NodePerson* b);//Merge b to the back of a

    //Fetch the smallest node
    void link(NodePerson* node, NodePerson* root);//Linking the node to the root node
    void consolidate();//Merge trees with the same degree
    void removeN(NodePerson* x);//Remove the node from the double-linked table
    NodePerson* extMinN();//remove the min tree form the list

    //update key value
    void updDegree(NodePerson *parent, int degree);//update degree value
	void cut(NodePerson* child, NodePerson* parent);//Strip the node from the child links of the parent parent and make the node a member of the root link table of the heap
	void casCut(NodePerson* y);//Continue cutting on the path from the reduced node to the root until you encounter a node that is either a root or an unmarked node.
    void decrease(NodePerson *node, MeTPerson* dkey);//decrease the key value
    void increase(NodePerson *node, MeTPerson* ikey);//increase the key value
    
    //destroy the heap
	void clear(NodePerson* x);//Destroying the Fibonacci heap
    void FibNode_PRINTN(NodePerson* x);  
};



#endif
