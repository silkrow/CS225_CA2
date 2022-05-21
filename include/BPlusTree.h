#ifndef BP_TREE
#define BP_TREE
#include "PersonDB.h"
#include "MeTPerson.h"
#include "BPNode.h"
#define order 4
#define LEAF true
#include <iostream>
#include <vector>
#include <queue>
using namespace std;


//template<typename MeTPerson*, typename MeTPerson*>
class BPTree{
private:
    Node *root;
    inline int keyIndex(Node *_node, MeTPerson* _key);
    inline int eqkeyIndex(Node *_node, MeTPerson* _key);
    inline pair<Node*, int> keyIndexInLeaf(MeTPerson* _key);
    inline pair<Node*, int> eqkeyIndexInLeaf(MeTPerson* _key);
    Node* splitLeaf(Node* _leaf);
    void createIndex(Node* _new_node, MeTPerson* _index);
    pair<Node*, MeTPerson*> splitNode(Node* _node);

public:
    BPTree();
    int sz;
    void insert(MeTPerson* _key);
    void erase(MeTPerson* _key);

    MeTPerson* find(MeTPerson* _key);
    void display();
    void scan();
    PerDB* PDB;
    MeTPerson* poptop(int num);
};

#endif