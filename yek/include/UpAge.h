#ifndef UP_AGE
#define UP_AGE

#include<queue>
#include "MeTPerson.h"

using namespace std;

class UpAge{
    public:
    UpAge();
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