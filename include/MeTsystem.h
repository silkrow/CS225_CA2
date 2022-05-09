#ifndef MET_SYS
#define MET_SYS

#include "InjPos.h"
#include "RegPos.h"
#include "MeTPerson.h"
#include "FibHeap.h"
#include "UpAge.h"
#include "PriLet.h"
#include<queue>

class MeTsystem {
	public:

		MeTsystem(/*int nOper*/);
        string startdate; // 
        // nOper: n operations in the test
		void run_MeT(string tt);
		void dailyRep_MeT(string tt);
        void weeklyRep_MeT(string tt);
        void monthlyRep_MeT(string tt);
		//void updateLocals(int idLoc, int newVal);
        
        void withdrawCheck(string tt);
        void dailyAgeCheck(string tt);
        int priorityAss(string tt); // return total number assigned
        int commomAss(int t); // return total number assigned: pri+common

        int numInj; // total number of injection pos
        int numReg; // total number of registration pos
        int nOp; // number of operations
        int nPer; // total number of person
        int totcap; // total capacity of injection pos
		int sortby; // by which priority the weekly report should be ordered.

        MeTPerson* dbPerson[10000005];
        RegPos* dbReg[1005];
        InjPos* dbInj[1005];       

        // choose injection place
        double Dis(InjPos* a,MeTPerson* b);
        void AssignInj();
        int assid[100005];
        int totass;
        
        // for report
        MeTPerson* repper[1000005];

        // for age check
        
        struct ageCmp{
           bool operator()(const MeTPerson &a, const MeTPerson &b)  {
                string s1 = a.dateBirth;
                string s2 = b.dateBirth;
                int by1,bm1,bd1,by2,bm2,bd2;
                by1=(s1[0]-'0')*1000+(s1[1]-'0')*100+(s1[2]-'0')*10+(s1[3]-'0');
                bm1=(s1[5]-'0')*10+(s1[6]-'0');
                bd1=(s1[8]-'0')*10+(s1[9]-'0');
                by2=(s2[0]-'0')*1000+(s2[1]-'0')*100+(s2[2]-'0')*10+(s2[3]-'0');
                bm2=(s2[5]-'0')*10+(s2[6]-'0');
                bd2=(s2[8]-'0')*10+(s2[9]-'0');
                if (by1<by2) return 0;
                if (by1==by2&&(bm1<bm2)) return 0;
                if (by1==by2&&bm1==bm2&&bd1<bd2)  return 0;
                return 1; 
                }
        };

        bool datecmp(string s1, string s2);

        FibHeap* FH;
        PriLet* PL;
        UpAge* UA;
};

#endif
