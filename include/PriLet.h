#ifndef PRI_LET
#define PRI_LET

#include<queue>
#include "MeTPerson.h"

using namespace std;

class PriLet{
    public:
    PriLet();
    int siz; // size of the pq
    MeTPerson* dbP[500005];

    void change(int a, int b);
    void insert(MeTPerson* newp);
    void pop(); // 
    void pushup(int x);
    bool cmp(MeTPerson* p1, MeTPerson* p2);
    void pushdown(int x);
    MeTPerson* top();
};

#endif