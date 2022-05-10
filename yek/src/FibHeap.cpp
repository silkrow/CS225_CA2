#include "FibHeap.h"
#include "MeTPerson.h"
#include "NodePerson.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

using namespace std;

FibHeap::FibHeap(){
    this->mNode = nullptr;
	this->totNum = 0;
    this->maxDegree = 0;
}

FibHeap::~FibHeap(){}

//create a new node
NodePerson* FibHeap::createN (MeTPerson* newp){
	NodePerson* newN = new NodePerson;//create a new node
	newN->keyPerson = newp;//initialize it
	newN->left = newN;
	newN->right = newN;
	newN->parent = nullptr;
	newN->child = nullptr;
	newN->degree = 0;
	newN->marked = false;
	return newN;
}

 //insert a new node with keyp as newp into the Fibonacci heap
NodePerson* FibHeap::insert(MeTPerson* newp){
    NodePerson* newn = createN(newp);//Create a new node with key newp
	insertN(newn);//insert it in the heap
	return newn;
}

//combine a new heap in current FibHeap
void FibHeap::combine(FibHeap* x){
	if (x==NULL) return;
	if ((this->mNode)==NULL){  //this has no min node but x have
		this->mNode = x->mNode;
        this->totNum = x->totNum;
        x=NULL;
		x->totNum=0;
	}
	else if ((x->mNode)==NULL){  //this has min node but x doesn't
        x=NULL;
		x->totNum=0;
    }
    else{    //add x to this
        catList(this->mNode, x->mNode);
		bool ret = this->mNode->keyPerson->cmpMeTPer(x->mNode->keyPerson);
        if (ret == 1){
            this->mNode=x->mNode;
		}
        this->totNum +=x->totNum;
		x=NULL;
		x->totNum=0;
    }

}

//remove the min node in fib heap, remove its PID
int FibHeap::removeM(){
	if (totNum == 0) return 0;
    NodePerson* m = mNode;
	int retid = m->keyPerson->pid;
    while (m->child != NULL){
        NodePerson* child = m->child;
        removeN(child);
        if (child->right == child){m->child =NULL;}
        else {m->child = child->right;}
        addNode(child, mNode);
        child->parent =NULL;
    }
    removeN(m); // Remove m from the root chain
    if (m->right==m){mNode=NULL;}// If m is the only node in the heap, set the smallest node of the heap to be null.
    else{                        // Otherwise, set the smallest node of the heap to a non-null node, and then adjust it.
        mNode = m->right;
        consolidate();
    }
    totNum--;
    m = NULL;
	return retid;
}

//update the keyp of x to newp
void FibHeap::updKey(NodePerson* x, MeTPerson* newp){
    bool cmp = x->keyPerson->cmpMeTPer(newp);
	if(cmp == 1){decrease(x, newp);}
    else increase(x, newp);
}

//decrease the key value
void FibHeap::decrease(NodePerson* n, MeTPerson* dkey){
    if (mNode== NULL || n == NULL) return;
	bool cmp1 = n->keyPerson->cmpMeTPer(dkey);
    if (cmp1 == 0){
        printf("decrease failed, the new key is larger than node key");
        return;
    }
    NodePerson* p = n->parent;
    n->keyPerson = dkey;
    if (p != NULL && ( n->keyPerson->cmpMeTPer(p->keyPerson) == 0)){// Strip the node from its parent parent and add the node to the root chain table
        cut(n, p);
        casCut(p);
    }
	bool cmp3 = n->keyPerson->cmpMeTPer(mNode->keyPerson);
    if (cmp3 == 0) {mNode = n;}// Update the minimum node       
}

void FibHeap::delNode(NodePerson* x){
    decrease(x, x->keyPerson);
	removeM();
}

//destroy the fib heap
void FibHeap::destroy(){
    clear(mNode);
}

//add the node before the root node in the double linked list
void FibHeap::addNode(NodePerson* node, NodePerson* root){
    node->left = root->left;
    root->left->right = node;
    node->right = root;
    root->left = node;
}

//Insert newn in the FibHeap
void FibHeap::insertN(NodePerson* newn){
	if (totNum == 0){mNode = newn;} //if there is no node in list, set new node as min node
    else{
        addNode(newn,mNode); //add new node before the min node
        bool cmp = newn->keyPerson->cmpMeTPer(mNode->keyPerson);
		if (cmp == 0){mNode=newn;} //if the node's key is smaller than min node's, set it as min node
    }
    totNum++;//update total number
}

//Merge b to the back of a
void FibHeap::catList(NodePerson* a, NodePerson* b){
	NodePerson* t; //merge b to the back of a
    t = a->right;
    a->right = b->right;
    b->right->left = a;
    b->right = t;
    t->left = b;
}

//Linking the node to the root node
void FibHeap::link(NodePerson* node, NodePerson* root){
    removeN(node);//remove the node from the list
	if (root->child==NULL) {root->child = node;}//if root don't have child, set node as its child
    else {addNode(node,root->child);}//else add node in the list of childs
    node->parent = root;
    root->degree++;
    node->marked = false;
}

//Merge trees with the same degree
void FibHeap::consolidate(){
    int maxDegree = (int)floor(log(totNum) / log(2.0));
    int D = maxDegree + 1;
    NodePerson** cons; //create a cons heap
	cons = (NodePerson**)malloc(sizeof(NodePerson)*D);
    for (int i = 0; i < D; i++){cons[i] = NULL;}//initialize
    while (mNode != NULL){ // Merge root nodes of the same degree so that the tree is unique for each degree
        NodePerson* x = extMinN(); // Fetch the smallest tree(the tree where the smallest node is located)in the heap         
        int d = x->degree; // Get the degree of the smallest tree                       
        while (cons[d] != NULL){   // cons[d] != null，means that there are trees with the same degree as x in the cons
            NodePerson* y = cons[d]; // y is the tree of the same degree as x                
            bool cmp1= x->keyPerson->cmpMeTPer(y->keyPerson);
			if (cmp1 == 1){  // ensure that the key value of x is smaller than y
                NodePerson* tmp = x;
                x = y;
                y = tmp;
            }
            link(y, x); // Link y to x   
            cons[d] =NULL;//update cons and degree
            d++;
        }
        cons[d] = x;
    }
    mNode =NULL;
    for (int i = 0; i < D; i++){  // Add the nodes in the cons back to the root table
        if (cons[i] != NULL){
            if (mNode == NULL){mNode = cons[i];}
            else{
                addNode(cons[i], mNode);
				bool cmp2 = cons[i]->keyPerson->cmpMeTPer(mNode->keyPerson);
                if (cmp2 == 0){mNode = cons[i];}
            }
        }
    }
}

//remove the min tree form the list
NodePerson* FibHeap::extMinN(){
    NodePerson* p = mNode;
    if (p == p->right){mNode = NULL;}
    else{
        removeN(p);
        mNode = p->right;
    }
    p->left = p->right = p;
    return p;
}

//Remove the node from the double-linked table
void FibHeap::removeN(NodePerson* x){
    if (x->right == x) {return;}
	x->left->right = x->right;
	x->right->left = x->left;
}

//update degree value
void FibHeap::updDegree(NodePerson *parent, int degree){
    parent->degree -= degree;
    if (parent->parent != NULL){updDegree(parent->parent,degree);}
}

//Strip the node from the child links of the parent parent and make the node a member of the root link table of the heap
void FibHeap::cut(NodePerson* n, NodePerson* parent){
    removeN(n);
    updDegree(parent, n->degree);
    if (n == n->right){parent->child = NULL;}//If node n doesn't have a brother
    else {parent->child = n->right;}
    n->parent = NULL;
    n->left = n->right = n;
    n->marked = false;
    addNode(n, mNode);// Add the tree where node n is located to the root link table
}

//Continue cutting on the path from the reduced node to the root until you encounter a node that is either a root or an unmarked node
void FibHeap::casCut(NodePerson* y){
    NodePerson* p = y->parent;
    if (p != NULL){
        if (y->marked == false){y->marked = true;}
        else{
            cut(y, p);
            casCut(p);
        }
    }
}

//increase the key value
void FibHeap::increase(NodePerson *n, MeTPerson* ikey){
    if (mNode== NULL || n == NULL) return;
	bool cmp1 = ikey->cmpMeTPer(n->keyPerson);
    if (cmp1 == 1){
        printf("increase failed, the new key is smaller than or equal to node key");
        return;
    }
    while (n->child != NULL){// Add each child of the node to the root chain
            NodePerson* c = n->child;
            removeN(c);               // Remove child from node's sub-link table
            if (c->right == c){n->child = NULL;}
            else{n->child = c->right;}
            addNode(c, mNode);    // Adding children to the root link table
            c->parent = NULL;
    }
    n->degree = 0;
    n->keyPerson = ikey;

    NodePerson* p = n->parent;
    if (p != NULL){
        cut(n, p);
        casCut(p);
    }
    else if (mNode == n){
        NodePerson* r = n->right;
        while (r != n){
			bool cmp2 = n->keyPerson->cmpMeTPer(r->keyPerson);
            if (cmp2 == 1){mNode = r;}
            r = r->right;
        }
    }
}

//Destroying the Fibonacci heap
void FibHeap::clear(NodePerson* x){
    if ( x != NULL ){
		NodePerson* a = x;  
		do{
			NodePerson* b = a; 
			a = a->right;
			clear(b->child);
			delete b;
		} while(a != x); 
	}
}

void FibHeap::FibNode_PRINTN(NodePerson* x)              
{
    NodePerson* p = NULL;
    if (x == NULL)
        return;
    p = x;
    do
    {
        printf("(");
        printf("%d", p->keyPerson->pid);
    
        if (p->child!=NULL )
        {
            FibNode_PRINTN(p->child);
        }
        printf(")");
        p = p->right;
    } while (p != x);

}


void FibHeap:: printFib()
{
    FibNode_PRINTN(this->mNode);
    printf("\n");
}
