#include "MeTPerson.h"
#include "FibHeap.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <cmath>
#include <time.h>

using namespace std;

MeTPerson::MeTPerson(){
    this->tmpSt = 0;
    this->idInjPos = 0;
    this->punishTime = 0;
    this->watime = 0;
    this->priLetter = 0;
    this->delmark = 0;
}

int MeTPerson::iniMeTPerson(int pid, int regpos, string tt){
    this->pid = pid;
    this->idRegpos = regpos;
    printf("Please enter the following information:\n");
    printf("Name:\n");
    getline(cin, this->name);
    string cinstr;
    printf("ID Number:\n");
    getline(cin, cinstr);
    int cnt = 0;
    while(cinstr.length() != 19){
        if(cnt > 5){
            printf("Registration failed!");
            return 0;
        }
        cnt++;
        printf("Invalid Id Number, please try again!");
        getline(cin, cinstr);
    }
    //this->idNumber = cinstr;
    printf("Phone Number:\n");
    getline(cin, this->phonen);
    printf("Wechat:\n");
    getline(cin, this->WeChatn);
    printf("Email:\n");
    getline(cin, this->eMailn);
    printf("Date of Birth(YYYY-MM-DD):\n");
    getline(cin, this->dateBirth);
    this->ageGroup = this->calAge(tt);
    printf("Profession:\n");
    cin>>this->profession;
    printf("Address Longitude:\n");
    cin>>this->addLon;
    printf("Address Latitude:\n");
    cin>>this->addLat;

    printf("Risk Status:\n");
    cnt = 0;
    cin>>this->riskSt;
    while(this->riskSt<0 || this->riskSt>2){
        if(cnt > 5){
            printf("Registration failed!");
            return 0;
        }
        cnt++;
        printf("Invalid Risk Status, please try again!");
        cin>>this->riskSt;
    }
    if(riskSt == 1) this->punishTime = 4;

    printf("Priority letter? 0-No 1-Yes\n");
    cin>>this->priLetter;
    if(this->priLetter == 1){
        printf("Priority letter deadline(YYYY-MM-DD)\n");
        getline(cin, this->priDate);
    }  
    //time_t curtime;
    //time(&curtime);
    //this->regDate = ctime(&curtime);
    return this->pid;
}

void MeTPerson::updInf(int typ, string newi){//This function is used to change this person's information
    switch(typ){
        case 1:{
            this->phonen = newi;
            break;
        }
        case 2:{
            this->WeChatn = newi;
            break;
        }
        case 3:{
            this->eMailn = newi;
            break;
        }
    }
}

void MeTPerson::updAdd(double newLo, double newLa){//This function is used to change this person's address
    this->addLon = newLo;
    this->addLat = newLa;
}
/*
This function calculates how many days between year1-mon1-day1 to year2-mon2-day2
*/
int MeTPerson::Dif(int year1,int mon1,int day1,int year2,int mon2,int day2){
    int mon[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    int i,m1=0,n1=0,n2=0,sum=0,t1=0,t2=0,t;
    for(i=0;i<12;i++){
        if(mon1==i+1) m1=mon[i]-day1;
    }
    for(i=mon1;i<12;i++) n1+=mon[i];
    for(i=0;i<mon2-1;i++) n2+=mon[i];
    if((year1 % 4 ==0 && year1 % 100 !=0) || year1 % 400 ==0){
        if(mon1<=2)
        if(day1!=29){t1++;}
    }
    if((year2 % 4 ==0 && year2 % 100 !=0) || year2 % 400 ==0){
        if(mon2>2)t2++;
    }
    for(i=year1+1,t=0;i<year2;i++){
        if((i % 4 ==0 || i % 400 ==0) && (i % 100 !=0)){t++;}
    }
    sum=m1+n1+n2+(year2-year1-1)*365+day2+t+t1+t2;
    return sum;  
}
/*
Input: year month day
Output: a formatted string representing date
*/
string MeTPerson::Tostring(int y,int m,int d){
    string s="";
    s +=(char)((y/1000)+'0');
    y%=1000;
    s +=(char)(y/100+'0');
    y%=100;
    s +=(char)(y/10+'0');
    s +=(char)(y%10+'0');
    s +='-';
    s +=(char)(m/10+'0');
    s +=(char)(m%10+'0');
    s +='-';
    s +=(char)(d/10+'0');
    s +=(char)(d%10+'0');
    return s;
}

/*
Input: string s, representing current date, in format YYYY-MM-DD, such as 2022-4-7
The person's ageGroup and nxtUpdate is calculated in this function
nxtUpdate: date for this person getting into the next date, in the same format as string s
*/
int MeTPerson::calAge(string s){
    int cury,curm,curd;
    cury=(s[0]-'0')*1000+(s[1]-'0')*100+(s[2]-'0')*10+(s[3]-'0');
    curm=(s[5]-'0')*10+(s[6]-'0');
    curd=(s[8]-'0')*10+(s[9]-'0');
    int by,bm,bd;
    by=(dateBirth[0]-'0')*1000+(dateBirth[1]-'0')*100+(dateBirth[2]-'0')*10+(dateBirth[3]-'0');
    bm=(dateBirth[5]-'0')*10+(dateBirth[6]-'0');
    bd=(dateBirth[8]-'0')*10+(dateBirth[9]-'0');
    int x1=cury-by;  int x2=curm-bm;  int x3=curd-bd;
    if (x1<=12) ageGroup=1;
    if (x1==12&&(x2>0||(x2==0&&x1>=0)))
        ageGroup++;
    if (x1>12&&x1<=18)
        ageGroup=2;
    if (x1==18&&(x2>0||(x2==0&&x1>=0)))
        ageGroup++;
    if (x1>18&&x1<=35)
        ageGroup=3;
    if (x1==35&&(x2>0||(x2==0&&x1>=0)))
        ageGroup++;
    if (x1>35&&x1<=50)
        ageGroup=4;
    if (x1==50&&(x2>0||(x2==0&&x1>=0)))
        ageGroup++;
    if (x1>50&&x1<=65)
        ageGroup=5;
    if (x1==65&&(x2>0||(x2==0&&x1>=0)))
        ageGroup++;
    if (x1>65&&x1<=75)
        ageGroup=6;
    if (x1==75&&(x2>0||(x2==0&&x1>=0)))
        ageGroup++;
    if (x1>75)   ageGroup=7;  

    switch (ageGroup){
    case 1:
        nxtUpdate=Tostring(by+12,bm,bd);
        break;
    case 2:
        nxtUpdate=Tostring(by+18,bm,bd);
        break;
    case 3:
        nxtUpdate=Tostring(by+35,bm,bd);
        break;
    case 4:
        nxtUpdate=Tostring(by+50,bm,bd);
        break;
    case 5:
        nxtUpdate=Tostring(by+65,bm,bd);
        break;
    case 6:
        nxtUpdate=Tostring(by+75,bm,bd);
        break;
    }
    return ageGroup;
}

/*
Input: string s, representing current date, in format YYYY-MM-DD, such as 2022-04-07
Value returned: an integer, the number of days passed since the person registered
*/
int MeTPerson::calwaitingtime(string s){
    int y1,y2,m1,m2,d1,d2;
    y1 = (this->regDate[0]-'0')*1000+(this->regDate[1]-'0')*100+(this->regDate[2]-'0')*10+(this->regDate[3]-'0');
    y2 = (s[0]-'0')*1000+(s[1]-'0')*100+(s[2]-'0')*10+(s[3]-'0');
    m1 = (this->regDate[5]-'0')*10 + (this->regDate[6]-'0');
    m2 = (s[5]-'0')*10 + (s[6]-'0');
    d1 = (this->regDate[8]-'0')*10 + (this->regDate[9]-'0');
    d2 = (s[8]-'0')*10 + (s[9]-'0');
    return Dif(y1,m1,d1,y2,m2,d2);
}
/*
Input: two person
This function compare two person's injection priority considering registeration date and punishtime
*/
bool MeTPerson::PersonComp(MeTPerson* p1,MeTPerson* p2){
    int by1,bm1,bd1,by2,bm2,bd2;
    double h1,m1,s1,h2,m2,s2;
    by1=(p1->regDate[0]-'0')*1000+(p1->regDate[1]-'0')*100+(p1->regDate[2]-'0')*10+(p1->regDate[3]-'0');
    bm1=(p1->regDate[5]-'0')*10+(p1->regDate[6]-'0');
    bd1=(p1->regDate[8]-'0')*10+(p1->regDate[9]-'0');

    h1=(p1->regDate[11]-'0')*10+(p1->regDate[12]-'0');
    m1=(p1->regDate[14]-'0')*10+(p1->regDate[15]-'0');
    s1=(p1->regDate[17]-'0')*10+(p1->regDate[18]-'0');

    by2=(p2->regDate[0]-'0')*1000+(p2->regDate[1]-'0')*100+(p2->regDate[2]-'0')*10+(p2->regDate[3]-'0');
    bm2=(p2->regDate[5]-'0')*10+(p2->regDate[6]-'0');
    bd2=(p2->regDate[8]-'0')*10+(p2->regDate[9]-'0');

    h2=(p2->regDate[11]-'0')*10+(p2->regDate[12]-'0');
    m2=(p2->regDate[14]-'0')*10+(p2->regDate[15]-'0');
    s2=(p2->regDate[17]-'0')*10+(p2->regDate[18]-'0');

    int x=Dif(by1,bm1,bd1,by2,bm2,bd2);
    m1+=s1/60; h1+=m1/60;
    m2+=s2/60; h2+=m2/60;
    double res=(double)x-h1/24+h2/24;

    res-=p1->punishTime*7;
    res+=p2->punishTime*7;
    if (res>0) return 0;
    else return 1;
}

int MeTPerson::calpriddl(string s){
    int y1,y2,m1,m2,d1,d2;
    y1 = (this->priDate[0]-'0')*1000+(this->priDate[1]-'0')*100+(this->priDate[2]-'0')*100+(this->priDate[3]-'0');
    y2 = (s[0]-'0')*1000+(s[1]-'0')*100+(s[2]-'0')*100+(s[3]-'0');
    m1 = (this->priDate[5]-'0')*10 + (this->priDate[6]-'0');
    m2 = (s[5]-'0')*10 + (s[6]-'0');
    d1 = (this->priDate[8]-'0')*10 + (this->priDate[9]-'0');
    d2 = (s[8]-'0')*10 + (s[9]-'0');
	if (dateCmp(this->priDate, s) == 0) return 0;
    return Dif(y2,m2,d2,y1,m1,d1);
}

void MeTPerson::updAge(string tt, FibHeap* FH){
    this->calAge(tt);
    //FH->updKey(this->Fibnode, this);
}

void MeTPerson::updPro(int newp, FibHeap* FH){
    this->profession = newp;
    FH->updKey(this->Fibnode, this);
}

void MeTPerson::updRiskSt(int newr, FibHeap* FH){
    this->riskSt = newr;
    FH->updKey(this->Fibnode, this);
}

void MeTPerson::updTmpSt(int newt){
    this->tmpSt = newt;
}

bool MeTPerson::dateCmp(string s1, string s2){
    // return 0 if s1 is ealier than s2
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

void MeTPerson::addPriL(string pDate){
    this->priLetter = 1;
    this->priDate = pDate;
}

bool MeTPerson::cmpMeTPer(MeTPerson* p2){
    // return 0 if p1 is prior than p2
    MeTPerson* p1 = this;
    // For Fib-del
    if(p1->delmark == 1) return 1;
    if(p2->delmark == 1) return 1;
    // FFFFFirst, check high risk
    if(p1->riskSt == 2 && p2->riskSt !=2) return 1;
    if(p1->riskSt != 2 && p2->riskSt == 2) return 0;
    
    // First, compare profession
    if(p1->profession < p2->profession) return 0;
    if(p1->profession > p2->profession) return 1;

    // Second, compare age group
    if(p1->ageGroup < p2->ageGroup) return 0;
    if(p1->ageGroup > p2->ageGroup) return 1;

    // Third, compare registration time
    return PersonComp(p1, p2);
}

void MeTPerson::withdraw(FibHeap* FH){
    if(this->tmpSt == 1){ 
        this->delmark = 1;
        FH->delNode(this->Fibnode);
    }
   	this->tmpSt = 3;
  	this->weekUpdate = 1;
   	this->monthUpdate = 1;
   	// 
}

void MeTPerson::reRegister(){
    if(riskSt == 0) punishTime+=2;
    this->tmpSt = 1;
    this->delmark = 0;
    this->weekUpdate = 1;
    this->monthUpdate = 1;
}

void MeTPerson::printPer(string tt, FILE* pfile){
    int waitingtime;
    if(tmpSt == 2 || tmpSt == 4) waitingtime = this->watime;
    else waitingtime = calwaitingtime(tt);
    fprintf(pfile, "%d %s %d %d %d %d %d\n",pid, name.c_str(), profession, ageGroup, waitingtime, 
			riskSt, idInjPos);
}
