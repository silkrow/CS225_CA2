#include "IntNode.h"
#include "MeTPerson.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include<algorithm>

using namespace std;

IntNode::IntNode(){// Initialization, create memory space for pointers
    this->maincnt=0;
    this->ofcnt=0;
    this->tombcnt=0;

	for (int i = 0; i < 13; i++)
		mainB[i] = new MeTPerson();
	for (int i = 0; i < 5; i++)
		ofB[i] = new MeTPerson();
}

void IntNode::Adjustof(){//sort the overflow block
    for (int i=0;i<ofcnt;i++)
        for (int j=i;j<ofcnt-1;j++)// because of its extremely small size, we use bubble sort
            if (ofB[j]->cmpMeTPer(ofB[j+1])){
                MeTPerson* tmp = new MeTPerson();
                tmp=ofB[j];
                ofB[j]=ofB[j+1];
                ofB[j+1]=tmp;
            }

}

int IntNode::Num(){//calculate the current number of person in this block
    return this->maincnt+this->ofcnt-this->tombcnt;
}

void IntNode::insertB(MeTPerson* tmp){//insert a person in this block
    //cout<<ofcnt<<endl;
    //ofcnt = 0;
    tmp->dbblock = this;
    if (this->ofcnt < 3){
    //    printf("fengzi\n");
        ofB[ofcnt++]=tmp;
        return;
    }
    this->sortB();//block is full, split and sort it(make the overflow block empty)
    ofB[ofcnt++]=tmp;
}

int IntNode::Find(MeTPerson* tmp){
    for (int i=0;i<ofcnt;i++)
        if (ofB[i]==tmp)
            return -2;	//if the person in the overflow block return -2
    int l=0,r=maincnt-1;
    if (l<=r){
        int mid=(l+r)/2;
        if (mainB[mid]==tmp){
            if (mainB[mid]->tombmark)
                return -1;
            return mid;//return its index
        }
        if (mainB[mid]->cmpMeTPer(tmp)==0)
            l=mid+1;
        else r=mid-1;
    }
    return -1;//not found, return -1
}

void IntNode::deleteB(MeTPerson* tmp){
    int fres=this->Find(tmp);
    if (fres==-1) return;
    if (fres==-2){
        int p=0;
        for (;p<ofcnt;p++)
            if (ofB[p]==tmp)
                break;
        for (int i=p;i<ofcnt-1;i++)
            ofB[i]=ofB[i+1];
        ofcnt--;
	//move the person out of overflow block
    }
    else{
        mainB[fres]->tombmark=1;//mark the person as tomb
        tombcnt++;
    }
    if (Num()<maincap/mergefactor)//elements are too few, merge the block with its neighbor
        mergeB();
    tmp->dbblock = NULL;
}

MeTPerson* IntNode::topPer(){//return the top element
    MeTPerson* res;
    res=NULL;
    int p1=0,p2=0;
    while (p1<maincnt&&mainB[p1]->tombmark)
        p1++;
    if (p1<maincnt)
        res=mainB[p1];
    for (int i=0;i<ofcnt;i++){
        if (res==NULL)
            res=ofB[i];
        else
            if (res->cmpMeTPer(ofB[i]))
                res=ofB[i];
    }
    return res;
}

void IntNode::mergeB(){//merge the block with its right neighbor
    this->sortB();
    if (this->rightNeighbor==this)
        return;
    
    IntNode* pt;
    pt=this->rightNeighbor;
    pt->sortB();
    
    MeTPerson* a[23];
    for(int i = 0; i < 23; i++){
        a[i] = new MeTPerson();
    }
    int p1=0,p2=0,p=0;

    while (p1<this->maincnt||p2<pt->maincnt){
        if (p1==this->maincnt){
            a[p++]=pt->mainB[p2++];
            continue;
        }
        if (p2==pt->maincnt){
            a[p++]=this->mainB[p1++];
            continue;
        }
        if (this->mainB[p1]->cmpMeTPer(pt->mainB[p2]))
            a[p++]=pt->mainB[p2++];
        else
            a[p++]=this->mainB[p1++];
    }
	//put all elements in the 2 block2 (overflow+main) into a sorted
    if (this->maincnt+pt->maincnt<=maincap){//can be put into 1 block
        IntNode* pt_next;
        pt_next=pt->rightNeighbor;

        this->rightNeighbor=pt_next;
        pt_next->leftNeighbor=this;

        this->maincnt=p;
        this->tombcnt=0;
        this->ofcnt=0;
        for (int i=0;i<p;i++)
            this->mainB[i]=a[i],a[i]->dbblock=this;
        return;
    }

    pt->maincnt=p/2;
    pt->ofcnt=0;
    pt->tombcnt=0;

    this->maincnt=(p+1)/2;
    this->ofcnt=0;
    this->tombcnt=0;

    for (int i=0;i<(p+1)/2;i++)
        this->mainB[i]=a[i],a[i]->dbblock=this;
    for (int i=0;i<p/2;i++)
        pt->mainB[i]=a[(p+1)/2+i],pt->mainB[i]->dbblock=pt;
	//distribute equally into 2 blocks
}
void IntNode::splitB(){
    MeTPerson* a[23];
    int p1=0,p2=0,p=0;
    Adjustof();
    while (p1<maincnt||p2<ofcnt){
        while (p1<maincnt&&mainB[p1]->tombmark){
            mainB[p1]->tombmark=0; p1++;
        }
        if (p1==maincnt){
            a[p++]=ofB[p2++];
            continue;
        }
        if (p2==ofcnt){
            a[p++]=mainB[p1++];
            continue;
        }
        if (mainB[p1]->cmpMeTPer(ofB[p2]))
            a[p++]=ofB[p2++];
        else
            a[p++]=mainB[p1++];
    }

    IntNode* pt_r=new IntNode;
    pt_r->maincnt=p/2;
    pt_r->ofcnt=0;
    pt_r->tombcnt=0;

    this->maincnt=(p+1)/2;
    this->ofcnt=0;
    this->tombcnt=0;

    for (int i=0;i<(p+1)/2;i++)
        this->mainB[i]=a[i],a[i]->dbblock=this;
    for (int i=0;i<p/2;i++)
        pt_r->mainB[i]=a[(p+1)/2+i],pt_r->mainB[i]->dbblock=pt_r;
    
    IntNode* pt_next;
    pt_next=this->rightNeighbor;
    this->rightNeighbor=pt_r;
    pt_r->leftNeighbor=this;
    pt_r->rightNeighbor=pt_next;
    pt_next->leftNeighbor=pt_r;
	//adjust the linked list of blocks
}

void IntNode::sortB(){
    if (maincnt+ofcnt>maincap){
        this->splitB();
        return;
    }
    MeTPerson* a[23];
    int p1=0,p2=0,p=0;
    Adjustof();
    while (p1<maincnt||p2<ofcnt){
        while (p1<maincnt&&mainB[p1]->tombmark){
            mainB[p1]->tombmark=0; p1++;
        }
        if (p1==maincnt){
            a[p++]=ofB[p2++];
            continue;
        }
        if (p2==ofcnt){
            a[p++]=mainB[p1++];
            continue;
        }
        if (mainB[p1]->cmpMeTPer(ofB[p2]))
            a[p++]=ofB[p2++];
        else
            a[p++]=mainB[p1++];
    }
    maincnt=p;
    ofcnt=0;
    tombcnt=0;

    for (int i=0;i<p;i++)
        mainB[i]=a[i];
}
