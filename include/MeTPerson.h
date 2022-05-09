#ifndef MET_PER
#define MET_PER

#include<cstring>
#include<cstdio>
#include<iostream>
#include<algorithm>
#include "NodePerson.h"
#include "FibHeap.h"

using namespace std;

class MeTPerson{
    public:
    MeTPerson();
    int iniMeTPerson(int pid, int regpos, string tt); // return pid or 0(failure)

    int pid; // unique id
    bool weekUpdate; // for week report
    bool monthUpdate; // for month report

    string name; 
    string phonen; 
    string WeChatn;
    string eMailn;
    int idNumber; // len=10 - ID number
    string dateBirth; // date of birth
    void updInf(int typ, string newi); // typ=1-phone 2-wechat 3-email

    double addLon; // address longitude, used for find the cloest inj pos
    double addLat; // address latitude
    void updAdd(double newLo, double newLa);

    int ageGroup; // current age group
    string nxtUpdate; // YYYY-MM-DD
    int calAge(string s); // calculate one's age group
    void updAge(string tt, FibHeap* FH);
    int Dif(int year1,int mon1,int day1,int year2,int mon2,int day2); // To calculate difference between dates. 
    string Tostring(int y,int m,int d); // To convert date infomation to string. 
    bool PersonComp(MeTPerson* p1,MeTPerson* p2); // Key comparison rule. 

    int profession; // 0~8, the smaller number means higher priority. 
    void updPro(int newp, FibHeap* FH);

    int riskSt; // medical risk status 0- no & low 1-mid 2-high 
    void updRiskSt(int newr, FibHeap* FH);
    int punishTime; // in weeks, for mid-risk and withdraw

    int tmpSt; // 0-In local 1-Centralized 2-Arranged injection 3-withdraw 4-Complete injection
    void updTmpSt(int newt);

    string regDate; // Date of registration
    bool dateCmp(string p1, string P2); // compare 2 date

    int idRegpos; // registration position id
    int idInjPos; // injection position id, 0 for No injections were scheduled

    bool priLetter; // priority letter 1-have
    string priDate; // priority letter DDL
    void addPriL(string pDate);
    int calpriddl(string s);

    //bool cmpMeTPer(MeTPerson* p1, MeTPerson* p2); // complete compare of 2 persons
    bool cmpMeTPer(MeTPerson* p);

    int calwaitingtime(string s); // return total waiting time in days
    int watime;
    
    void withdraw(FibHeap* FH);
    void reRegister();
    
    void printPer(string tt, FILE*);

    bool delmark;
    
    NodePerson* Fibnode;
};

#endif
