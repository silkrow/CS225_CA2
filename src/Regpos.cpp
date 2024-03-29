#include "RegPos.h"
#include "MeTPerson.h"
#include "FibHeap.h"
#include "MeTsystem.h"
#include "BPlusTree.h"
#include "BTree.h"
#include "FibHeap.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

/*Class RegPos represents registeration position*/

RegPos::RegPos(int id){//Initialization
    this->RegID = id;
	this->cur = 0;
	this->cnt = 0;
}

void RegPos::forwardInf(FibHeap* FH, BPTree* B1, BTree* B2, MeTPerson** db){//Upload information in local queue into centralized queue
    for(int i = cur; i < cnt; i++){
		if (db[locList[i]]->tmpSt == 3) continue;
        NodePerson* newp = FH->insert(db[locList[i]]);
        db[locList[i]]->Fibnode = newp;
        B1->insert(db[locList[i]]);
       // printf("inb2\n");
        B2->insert(db[locList[i]]);
      //  printf("inb222\n");
        db[locList[i]]->tmpSt = 1;
    }
    cur = cnt;
}
