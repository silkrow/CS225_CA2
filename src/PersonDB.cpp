#include "PersonDB.h"
#include "IntNode.h"
#include "MeTPerson.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

using namespace std;

PerDB::PerDB(){
    topNode = createBlock();
}

IntNode* PerDB::createBlock(){//create a new block in the PerDB system
    IntNode* pt = new IntNode();
    pt->maincnt=0;
    pt->ofcnt=0;
    pt->tombcnt=0;
    pt->leftNeighbor=pt;
    pt->rightNeighbor=pt;
    return pt;
}

MeTPerson* PerDB::popTopone(){//pop the topest element
    IntNode* top = this->topNode;
    top->sortB();
    MeTPerson* ret;
    if(top->maincnt == 0){
        ret = NULL;
        return ret;
    }
    ret = top->mainB[0];
    top->deleteB(ret);
    return ret;

}

MeTPerson* PerDB::popTopn(int num){//pop the topest n element
    IntNode* pt=topNode;
    MeTPerson* Head=new MeTPerson();
    MeTPerson* Tail=new MeTPerson();
    Head=NULL;//The head and tail for the result linked list
    Tail=NULL;
    bool f1=0;

    while (num){
        if (f1&&pt==topNode)
            break;
        if (pt==topNode)
            f1=1;
        if (pt->Num()<=num){//If the whole block should be pop
            num-=pt->Num();

            int p1=0,p2=0;
            pt->Adjustof();
            while (p1<pt->maincnt||p2<pt->ofcnt){
                while (p1<pt->maincnt&&pt->mainB[p1]->tombmark)
                    pt->mainB[p1++]->tombmark=0;
                if (p1==pt->maincnt){
                    if (Head==NULL){
                        Head=pt->ofB[p2++];
                        Tail=Head;// add the element in the tail
                    }
                    else{
                        Tail->lhc=pt->ofB[p2++];
                        Tail=Tail->lhc;
                    }
                    continue;
                }
                if (p2==pt->ofcnt){
                    if (Head==NULL){
                        Head=pt->mainB[p1++];
                        Tail=Head;
                    }
                    else{
                        Tail->lhc=pt->mainB[p1++];
                        Tail=Tail->lhc;
                    }
                    continue;
                }
                if (pt->mainB[p1]->cmpMeTPer(pt->ofB[p2])){
                    if (Head==NULL){
                        Head=pt->ofB[p2++];
                        Tail=Head;
                    }
                    else{
                        Tail->lhc=pt->ofB[p2++];
                        Tail=Tail->lhc;
                    }
                    continue;
                }
                else{
                    if (Head==NULL){
                        Head=pt->mainB[p1++];
                        Tail=Head;
                    }
                    else{
                        Tail->lhc=pt->mainB[p1++];
                        Tail=Tail->lhc;
                    }
                    continue;                  
                }
            }

            if (pt->rightNeighbor==pt){
                topNode = createBlock();
                break;
            }

            IntNode* pt_next=pt->rightNeighbor;
            IntNode* pt_pre=pt->leftNeighbor;
            pt_next->leftNeighbor=pt_pre;
            pt_pre->rightNeighbor=pt_next;

            pt=pt_next;
        }
        else{
            while (num){
                if (Head==NULL){
                    Head=pt->topPer();
                    Tail=Head;
                }
                else{
                    Tail->lhc=pt->topPer();
                    Tail=Tail->lhc;
                }
                pt->deleteB(Tail);
                num--;
            }
            //If only a few elements remaining need poping, pop it 1 by 1
        }
    }
    return Head;
}

 
void PerDB::insertPer(MeTPerson* tmp, IntNode* block){ 
    //block->insertB(tmp);
}
void PerDB::deletePer(MeTPerson* tmp, IntNode* block){
    block->deleteB(tmp);
}
