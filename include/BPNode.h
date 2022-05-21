#ifndef BP_NODE
#define BP_NODE
#include "PersonDB.h"
#include "MeTPerson.h"

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Node{
public:
    bool leaf;
    Node *parent;   //for non-root only
    Node *next;     //for leaf only
    Node *last;     //for leaf only
    vector<MeTPerson*> key;
    vector<Node*> ptr2node;    //for non-leaf only
    vector<MeTPerson*> ptr2val;     //for leaf only
    Node(bool _leaf = false);
};

#endif