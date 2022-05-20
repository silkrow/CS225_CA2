#include "PersonDB.h"
#include "IntNode.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

using namespace std;

PerDB::PerDB(){
    topNode=NULL;
}

void PerDB::createBlock(){
    IntNode* pt;
    pt->maincnt=0;
    pt->ofcnt=0;
    pt->tombcnt=0;
    pt->leftNeighbor=pt;
    pt->rightNeighbor=pt;
    topNode=pt;
}

MeTPerson* PerDB::popTopn(int num){
    IntNode* pt=topNode;
    MeTPerson* Guard;
    Guard->lhc=NULL;
    MeTPerson* cur;
    cur=Guard;
    bool f1=0;

    while (num){
        if (f1&&pt==topNode)
            break;
        if (pt==topNode)
            f1=1;
        if (pt->Num()<=num){
            int p1=0,p2=0;
            while (p1<pt->maincnt||p2<pt->ofcnt){
                pt->Adjustof();
                while (p1<pt->maincnt&&pt->mainB[p1]->tombmark)
                    p1++;
                if (p1==pt->maincnt){
                    cur->lhc=pt->ofB[p2++];
                    cur=cur->lhc;
                    continue;
                }
                if (p2==pt->maincnt){
                    cur->lhc=pt->mainB[p1++];
                    cur=cur->lhc;
                    continue;
                }
                if (pt->mainB[p1]->cmpMeTPer(pt->ofB[p2])){
                    cur->lhc=pt->ofB[p2++];
                    cur=cur->lhc;
                    continue;
                }
                else{
                    cur->lhc=pt->mainB[p1++];
                    cur=cur->lhc;
                    continue;                  
                }
            }
            num-=pt->Num();

            if (pt->rightNeighbor==pt){
                topNode=NULL;
                createBlock();
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
                cur->lhc=pt->topPer();
                cur=cur->lhc;
                pt->deleteB(cur);
                num--;
            }
        }
    }
    return Guard->lhc;
}

 
void PerDB::insertPer(MeTPerson* tmp, IntNode* block){
    IntNode* pt=topNode;
    while (pt!=block)
        pt=pt->rightNeighbor;
    pt->insertB(tmp);
}
void PerDB::deletePer(MeTPerson* tmp, IntNode* block){
    IntNode* pt=topNode;
    while (pt!=block)
        pt=pt->rightNeighbor;
    pt->deleteB(tmp);
}
